// SPDX-License-Identifier: GPL-2.0
/*
 * ALSA SoC Synopsys I2S Audio Layer
 *
 * sound/soc/dwc/designware_i2s.c
 *
 * Copyright (C) 2010 ST Microelectronics
 * Rajeev Kumar <rajeevkumar.linux@gmail.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 *
 */
/*****************************************************************************
 * ESWIN i2s driver
 *
 * Copyright 2024, Beijing ESWIN Computing Technology Co., Ltd.. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Authors: DengLei <denglei@eswincomputing.com>
 */

#include <sound/pcm.h>
#include <linux/device.h>
#include <sound/soc.h>
#include <linux/pm_runtime.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <sound/pcm_params.h>
#include <asm/io.h>
#include <sound/asound.h>
#include <sound/designware_i2s.h>
#include <linux/irqreturn.h>
#include <linux/clk.h>
#include <sound/soc-dai.h>
#include <linux/ioport.h>
#include <linux/err.h>
#include <linux/pm.h>
#include <linux/mfd/syscon.h>
#include <linux/reset.h>
#include <linux/dma-map-ops.h>
#include <linux/of_address.h>
#include "esw-i2s.h"
#include "esw-audio-proc.h"
#include "esw-dai.h"

#define VO_MCLK_DIVSOR_MASK    0xff0
#define VO_MCLK_DIVSOR_OFFSET  4

#define MAX_SAMPLE_RATE_SUPPORT (192000UL)
#define MAX_SAMPLE_RATE_CLK (MAX_SAMPLE_RATE_SUPPORT * 32 * 2) // 32 bits, 2channels

#define DIE0_VO_TOP_CSR        0x50280000UL
#define DIE1_VO_TOP_CSR        0x70280000UL
#define VO_I2S0_DIV_NUM        0x2000
#define VO_I2S1_DIV_NUM        0x2004
#define VO_I2S2_DIV_NUM        0x2008
#define DIV_NUM_MASK           0x1f

#define ESW_I2S_RATES (SNDRV_PCM_RATE_192000 | \
			SNDRV_PCM_RATE_96000 | \
			SNDRV_PCM_RATE_48000 | \
			SNDRV_PCM_RATE_44100 | \
			SNDRV_PCM_RATE_32000 | \
			SNDRV_PCM_RATE_16000 | \
			SNDRV_PCM_RATE_8000)
#define ESW_I2S_FORMATS (SNDRV_PCM_FMTBIT_S32_LE | \
			SNDRV_PCM_FMTBIT_S16_LE | \
			SNDRV_PCM_FMTBIT_S24_LE | \
			SNDRV_PCM_FMTBIT_S24_3LE)

#define DIE0_I2S0_IO_ADDR 0x51600124
#define DIE1_I2S0_IO_ADDR 0x71600124

#define DIE0_DAI_DRIVER_OFFSET 0
#define DIE1_DAI_DRIVER_OFFSET 4

#define APLL_HIGH_FREQ  983040000
#define APLL_LOW_FREQ   225792000

#define D0_HDMI_DAI_NAME "i2s0-hdmi"
#define D1_HDMI_DAI_NAME "d1-i2s0-hdmi"

static int enable_441k[2] = {0, 0};
static int i2s_enable_cnt[2] = {0, 0};

static u32 dmaen_txch[] = {
	DMAEN_TXCH_0,
	DMAEN_TXCH_1,
	DMAEN_TXCH_2,
	DMAEN_TXCH_3
};

static u32 dmaen_rxch[] = {
	DMAEN_RXCH_0,
	DMAEN_RXCH_1,
	DMAEN_RXCH_2,
	DMAEN_RXCH_3
};

/* Maximum bit resolution of a channel - not uniformly spaced */
static const u32 fifo_width[COMP_MAX_WORDSIZE] = {
	12, 16, 20, 24, 32, 0, 0, 0
};

/* Width of (DMA) bus */
static const u32 bus_widths[COMP_MAX_DATA_WIDTH] = {
	DMA_SLAVE_BUSWIDTH_1_BYTE,
	DMA_SLAVE_BUSWIDTH_2_BYTES,
	DMA_SLAVE_BUSWIDTH_4_BYTES,
	DMA_SLAVE_BUSWIDTH_UNDEFINED
};

int i2s_get_nid(struct device *dev)
{
	int nid = dev_to_node(dev);

	if (nid == NUMA_NO_NODE) {
	#ifdef CONFIG_NUMA
		dev_err(dev, "%s:%d, NUMA_NO_NODE\n", __func__, __LINE__);
		return NUMA_NO_NODE;
	#else
		dev_info(dev, "nid:NUMA_NO_NODE, single DIE\n");
		nid = 0;
	#endif
	} else {
		dev_info(dev, "nid:%d\n", nid);
	}
	return nid;
}

static int set_mclk(struct device *dev, struct i2s_dev *i2s_drvdata)
{
	int ret;

	i2s_drvdata->mclk = devm_clk_get(dev, "mclk");
	if (IS_ERR(i2s_drvdata->mclk)) {
		dev_err(dev, "Failed to get mclock: %ld\n", PTR_ERR(i2s_drvdata->mclk));
		return PTR_ERR(i2s_drvdata->mclk);
	}

	ret = clk_prepare_enable(i2s_drvdata->mclk);
	if (ret < 0) {
		dev_err(dev, "Failed to enable mclock: %d\n", ret);
		return ret;
	}

	/* only set once */
	if (of_node_name_prefix(dev->of_node, "i2s0")) {
		ret = clk_set_rate(i2s_drvdata->mclk, MAX_SAMPLE_RATE_CLK);
		if (ret) {
			dev_err(dev, "Can't set I2S mclock rate: %d\n", ret);
			clk_disable_unprepare(i2s_drvdata->mclk);
			return ret;
		}
	}

	i2s_drvdata->apll_clk = devm_clk_get(dev, "apll");
	if (IS_ERR(i2s_drvdata->apll_clk)) {
		dev_err(dev, "Failed to get apll clock: %ld\n", PTR_ERR(i2s_drvdata->apll_clk));
		clk_disable_unprepare(i2s_drvdata->mclk);
		return PTR_ERR(i2s_drvdata->apll_clk);
	}

	return 0;
}


unsigned long get_vo_top_csr(int nid)
{
	return (nid == 1) ? DIE1_VO_TOP_CSR : DIE0_VO_TOP_CSR;
}

unsigned long get_i2s0_io_addr(int nid)
{
	return (nid == 1) ? DIE1_I2S0_IO_ADDR : DIE0_I2S0_IO_ADDR;
}

static inline u32 i2s_read_reg(void *io_base, int reg)
{
	return readl((char *)io_base + reg);
}

static inline void i2s_write_reg(void *io_base, int reg, u32 val)
{
	writel(val, (char *)io_base + reg);
}

static inline void i2s_disable_channels(struct i2s_dev *i2s_drvdata, u32 stream)
{
	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		i2s_write_reg(i2s_drvdata->i2s_base, TER(0), 0);
	} else {
		i2s_write_reg(i2s_drvdata->i2s_base, RER(0), 0);
	}
}

static void i2s_config(struct i2s_dev *i2s_drvdata, int stream)
{
	u32 ch_reg;
	struct i2s_clk_config_data *config = &i2s_drvdata->config;
	i2s_disable_channels(i2s_drvdata, stream);
	for (ch_reg = 0; ch_reg < (config->chan_nr / 2); ch_reg++) {
		if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
			i2s_write_reg(i2s_drvdata->i2s_base, TCR(ch_reg),
				      i2s_drvdata->xfer_resolution);
			i2s_write_reg(i2s_drvdata->i2s_base, TFCR(ch_reg),
				      i2s_drvdata->fifo_th - 1);
			i2s_write_reg(i2s_drvdata->i2s_base, TER(ch_reg), 1);
		} else {
			i2s_write_reg(i2s_drvdata->i2s_base, RCR(ch_reg),
				      i2s_drvdata->xfer_resolution);
			i2s_write_reg(i2s_drvdata->i2s_base, RFCR(ch_reg),
				      i2s_drvdata->fifo_th - 1);
			i2s_write_reg(i2s_drvdata->i2s_base, RER(ch_reg), 1);
		}
    }
}

static inline void i2s_enable_irqs(struct i2s_dev *i2s_drvdata, u32 stream,
				   int chan_nr)
{
	u32 i, irq;
	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		for (i = 0; i < (chan_nr / 2); i++) {
			irq = i2s_read_reg(i2s_drvdata->i2s_base, IMR(i));
			i2s_write_reg(i2s_drvdata->i2s_base, IMR(i), irq & ~0x30);
		}
	} else {
		for (i = 0; i < (chan_nr / 2); i++) {
			irq = i2s_read_reg(i2s_drvdata->i2s_base, IMR(i));
			i2s_write_reg(i2s_drvdata->i2s_base, IMR(i), irq & ~0x03);
		}
    }
}

static inline void i2s_enable_dedicated_dma(struct i2s_dev *i2s_drvdata, u32 stream,
				   int chan_nr)
{
	u32 i, dmacr;
	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		for (i = 0; i < (chan_nr / 2); i++) {
			dmacr = i2s_read_reg(i2s_drvdata->i2s_base, DMACR);
			i2s_write_reg(i2s_drvdata->i2s_base, DMACR, dmacr | dmaen_txch[i]);
		}
	} else {
		for (i = 0; i < (chan_nr / 2); i++) {
			dmacr = i2s_read_reg(i2s_drvdata->i2s_base, DMACR);
			i2s_write_reg(i2s_drvdata->i2s_base, DMACR, dmacr | dmaen_rxch[i]);
		}
    }
}

static inline void i2s_disable_dedicated_dma(struct i2s_dev *i2s_drvdata, u32 stream,
				   int chan_nr)
{
	u32 i, dmacr;
	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		for (i = 0; i < (chan_nr / 2); i++) {
			dmacr = i2s_read_reg(i2s_drvdata->i2s_base, DMACR);
			i2s_write_reg(i2s_drvdata->i2s_base, DMACR, dmacr & ~dmaen_txch[i]);
		}
	} else {
		for (i = 0; i < (chan_nr / 2); i++) {
			dmacr = i2s_read_reg(i2s_drvdata->i2s_base, DMACR);
			i2s_write_reg(i2s_drvdata->i2s_base, DMACR, dmacr & ~dmaen_rxch[i]);
		}
    }
}

static inline void i2s_enable_combined_dma(struct i2s_dev *i2s_drvdata, u32 stream)
{
	u32 dmacr;
	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		dmacr = i2s_read_reg(i2s_drvdata->i2s_base, DMACR);
		i2s_write_reg(i2s_drvdata->i2s_base, DMACR, dmacr | DMAEN_TXBLOCK);

	} else {
		dmacr = i2s_read_reg(i2s_drvdata->i2s_base, DMACR);
		i2s_write_reg(i2s_drvdata->i2s_base, DMACR, dmacr | DMAEN_RXBLOCK);
    }
}

static inline void i2s_disable_combined_dma(struct i2s_dev *i2s_drvdata, u32 stream)
{
	u32 dmacr;
	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		dmacr = i2s_read_reg(i2s_drvdata->i2s_base, DMACR);
		i2s_write_reg(i2s_drvdata->i2s_base, DMACR, dmacr & ~DMAEN_TXBLOCK);

	} else {
		dmacr = i2s_read_reg(i2s_drvdata->i2s_base, DMACR);
		i2s_write_reg(i2s_drvdata->i2s_base, DMACR, dmacr & ~DMAEN_RXBLOCK);
    }
}

static void i2s_start(struct i2s_dev *i2s_drvdata,
		      struct snd_pcm_substream *substream)
{
	struct i2s_clk_config_data *config = &i2s_drvdata->config;
	i2s_write_reg(i2s_drvdata->i2s_base, IER, 1);
	if (i2s_drvdata->use_pio) {
		i2s_enable_irqs(i2s_drvdata, substream->stream, config->chan_nr);
	}
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		i2s_write_reg(i2s_drvdata->i2s_base, ITER, 1);
	} else {
		i2s_write_reg(i2s_drvdata->i2s_base, IRER, 1);
	}
	if (!i2s_drvdata->use_pio) {
		i2s_enable_dedicated_dma(i2s_drvdata, substream->stream, config->chan_nr);
	}
	i2s_write_reg(i2s_drvdata->i2s_base, CER, 1);
}

static inline void i2s_clear_irqs(struct i2s_dev *i2s_drvdata, u32 stream)
{
	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		i2s_read_reg(i2s_drvdata->i2s_base, TOR(0));
	} else {
		i2s_read_reg(i2s_drvdata->i2s_base, ROR(0));
	}
}

static inline void i2s_disable_irqs(struct i2s_dev *i2s_drvdata, u32 stream,
				    int chan_nr)
{
	u32 i, irq;
	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		for (i = 0; i < (chan_nr / 2); i++) {
			irq = i2s_read_reg(i2s_drvdata->i2s_base, IMR(i));
			i2s_write_reg(i2s_drvdata->i2s_base, IMR(i), irq | 0x30);
		}
	} else {
		for (i = 0; i < (chan_nr / 2); i++) {
			irq = i2s_read_reg(i2s_drvdata->i2s_base, IMR(i));
			i2s_write_reg(i2s_drvdata->i2s_base, IMR(i), irq | 0x03);
		}
	}
}

static void i2s_stop(struct i2s_dev *i2s_drvdata,
		struct snd_pcm_substream *substream)
{
	if (i2s_drvdata->use_pio) {
		i2s_clear_irqs(i2s_drvdata, substream->stream);
	}
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		i2s_write_reg(i2s_drvdata->i2s_base, ITER, 0);
	} else {
		i2s_write_reg(i2s_drvdata->i2s_base, IRER, 0);
	}
	if (i2s_drvdata->use_pio) {
		i2s_disable_irqs(i2s_drvdata, substream->stream, 2);
	} else {
		i2s_disable_dedicated_dma(i2s_drvdata, substream->stream, 2);
	}
	if (!i2s_drvdata->active) {
		i2s_write_reg(i2s_drvdata->i2s_base, CER, 0);
		i2s_write_reg(i2s_drvdata->i2s_base, IER, 0);
	}
}

static int i2s_configure_res_by_dt(struct i2s_dev *dev,
								   struct resource *res)
{
	u32 comp1 = i2s_read_reg(dev->i2s_base, I2S_COMP_PARAM_1);
	u32 comp2 = i2s_read_reg(dev->i2s_base, I2S_COMP_PARAM_2);
	u32 fifo_depth;
	u32 idx;
	u32 idx2;

	dev_info(dev->dev, "comp1:0x%x, comp2:0x%x\n", comp1, comp2);
	fifo_depth = 1 << (1 + COMP1_FIFO_DEPTH_GLOBAL(comp1));
	idx = COMP1_APB_DATA_WIDTH(comp1);

	if (WARN_ON(idx >= ARRAY_SIZE(bus_widths))) {
		dev_err(dev->dev, "idx:%d inval\n", idx);
		return -EINVAL;
	}

	if (COMP1_MODE_EN(comp1)) {
		dev_dbg(dev->dev, "eswin: i2s master mode supported\n");
		dev->capability |= DW_I2S_MASTER;
	} else {
		dev_dbg(dev->dev, "eswin: i2s slave mode supported\n");
		dev->capability |= DW_I2S_SLAVE;
	}
	dev->fifo_th = fifo_depth / 2;

	if (COMP1_TX_ENABLED(comp1)) {
		idx2 = COMP1_TX_WORDSIZE_0(comp1);
		dev->capability |= DWC_I2S_PLAY;
		/* only  configure Combined DMA addr, Our scenario is not Dedicated DMA case */
		dev->play_dma_data.addr_width = bus_widths[idx];
		dev->play_dma_data.fifo_size = fifo_depth *
			(fifo_width[idx2]) >> 3;
		dev->play_dma_data.addr = res->start + TXDMA_CH(0);
		dev->play_dma_data.max_burst = 16;
	}
	if (COMP1_RX_ENABLED(comp1)) {
		idx2 = COMP2_RX_WORDSIZE_0(comp2);
		dev->capability |= DWC_I2S_RECORD;
		/* only  configure Combined DMA addr, Our scenario is not Dedicated DMA case */
		dev->capture_dma_data.addr_width = bus_widths[idx];
		dev->capture_dma_data.fifo_size = fifo_depth *
			(fifo_width[idx2]) >> 3;
		dev->capture_dma_data.addr = res->start + RXDMA_CH(0);
		dev->capture_dma_data.max_burst = 16;
	}

	return 0;
}

static int i2s_startup(struct snd_pcm_substream *substream,
		struct snd_soc_dai *cpu_dai)
{
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct snd_soc_dai_link *dai_link = rtd->dai_link;

	dai_link->trigger_stop = SND_SOC_TRIGGER_ORDER_LDC;

	return 0;
}

static int i2s_hw_params(struct snd_pcm_substream *substream,
		struct snd_pcm_hw_params *params, struct snd_soc_dai *dai)
{
	struct i2s_dev *i2s_drvdata = snd_soc_dai_get_drvdata(dai);
	struct i2s_clk_config_data *config = &i2s_drvdata->config;
	uint32_t div_num = 0;
	uint32_t div_num_reg;
	int ret;

	dev_dbg(i2s_drvdata->dev, "sample rate:%d, chan:%d, width:%d\n",
			 params_rate(params), params_channels(params), params_width(params));
	switch (params_format(params)) {
	case SNDRV_PCM_FORMAT_S16_LE:
		config->data_width = 16;
		i2s_drvdata->ccr = CLOCK_CYCLES_32 << CCR_WSS_POS |
					NO_CLOCK_GATING;
		i2s_drvdata->xfer_resolution = RESOLUTION_16_BIT;
		break;
	case SNDRV_PCM_FORMAT_S24_LE:
	case SNDRV_PCM_FORMAT_S24_3LE:
		config->data_width = 24;
		i2s_drvdata->ccr = CLOCK_CYCLES_32 << CCR_WSS_POS |
					NO_CLOCK_GATING;
		i2s_drvdata->xfer_resolution = RESOLUTION_24_BIT;
		break;
	case SNDRV_PCM_FORMAT_S32_LE:
		config->data_width = 32;
		i2s_drvdata->ccr = CLOCK_CYCLES_32 << CCR_WSS_POS |
					NO_CLOCK_GATING;
		i2s_drvdata->xfer_resolution = RESOLUTION_32_BIT;
		break;
	default:
		dev_err(i2s_drvdata->dev, "eswin-i2s: unsupported PCM fmt");
		return -EINVAL;
	}
	config->chan_nr = MAX_CHANNEL_NUM;
	switch (config->chan_nr) {
	case TWO_CHANNEL_SUPPORT:
		break;
	default:
		dev_err(i2s_drvdata->dev, "channel not supported\n");
		return -EINVAL;
	}
	i2s_config(i2s_drvdata, substream->stream);
	i2s_write_reg(i2s_drvdata->i2s_base, CCR, i2s_drvdata->ccr);
	config->sample_rate = params_rate(params);
	if (i2s_drvdata->capability & DW_I2S_MASTER) {
		if (config->sample_rate == 44100) {
			if (!enable_441k[i2s_drvdata->nid]) {
				if (!i2s_enable_cnt[i2s_drvdata->nid]) {
					ret = clk_set_rate(i2s_drvdata->mclk, MAX_SAMPLE_RATE_CLK * 2);
					if (ret) {
						dev_err(i2s_drvdata->dev, "Can't set I2S mclock rate: %d\n", ret);
						return ret;
					}
					ret = clk_set_rate(i2s_drvdata->apll_clk, APLL_LOW_FREQ);
					if (ret) {
						dev_err(i2s_drvdata->dev, "Can't set I2S apll clock rate: %d\n", ret);
						return ret;
					}
					enable_441k[i2s_drvdata->nid] = 1;
					div_num_reg = i2s_read_reg(i2s_drvdata->i2s_div_base, 0) & ~DIV_NUM_MASK;
					div_num_reg |= 1;
					i2s_write_reg(i2s_drvdata->i2s_div_base, 0, div_num_reg);
					i2s_drvdata->i2s_div_num = 1;
					dev_dbg(i2s_drvdata->dev, "apll rate:%ld\n", clk_get_rate(i2s_drvdata->apll_clk));
				} else {
					dev_err(i2s_drvdata->dev, "Other sample rate audio is playing.\n");
					return -EINVAL;
				}
			} else {
				div_num_reg = i2s_read_reg(i2s_drvdata->i2s_div_base, 0) & ~DIV_NUM_MASK;
				div_num_reg |= 1;
				i2s_write_reg(i2s_drvdata->i2s_div_base, 0, div_num_reg);
				i2s_drvdata->i2s_div_num = 1;
			}
		} else {
			if (enable_441k[i2s_drvdata->nid]) {
				if (!i2s_enable_cnt[i2s_drvdata->nid]) {
					ret = clk_set_rate(i2s_drvdata->apll_clk, APLL_HIGH_FREQ);
					if (ret) {
						dev_err(i2s_drvdata->dev, "Can't set I2S apll clock rate: %d\n", ret);
						return ret;
					}
					ret = clk_set_rate(i2s_drvdata->mclk, MAX_SAMPLE_RATE_CLK);
					if (ret) {
						dev_err(i2s_drvdata->dev, "Can't set I2S mclock rate: %d\n", ret);
						return ret;
					}
					enable_441k[i2s_drvdata->nid] = 0;
					dev_dbg(i2s_drvdata->dev, "apll rate:%ld\n", clk_get_rate(i2s_drvdata->apll_clk));
				} else {
					dev_err(i2s_drvdata->dev, "44.1khz audio is playing.\n");
					return -EINVAL;
				}
			}
			if (MAX_SAMPLE_RATE_SUPPORT % config->sample_rate != 0) {
				dev_err(i2s_drvdata->dev, "Not support sample rate: %d\n", config->sample_rate);
				return -EINVAL;
			}

			div_num = MAX_SAMPLE_RATE_SUPPORT / config->sample_rate - 1;

			if (i2s_drvdata->active) {
				if (i2s_drvdata->i2s_div_num != div_num) {
					dev_err(i2s_drvdata->dev, "Not support the playback and capture clocks are different\n");
					return -EINVAL;
				}
			} else {
				div_num_reg = i2s_read_reg(i2s_drvdata->i2s_div_base, 0) & ~DIV_NUM_MASK;
				div_num_reg |= div_num;
				dev_dbg(i2s_drvdata->dev, "div num:0x%x\n", div_num);
				i2s_drvdata->i2s_div_num = div_num;
				i2s_write_reg(i2s_drvdata->i2s_div_base, 0, div_num_reg);
			}
		}
	}

	return 0;
}

static int i2s_prepare(struct snd_pcm_substream *substream,
			  struct snd_soc_dai *dai)
{
	struct i2s_dev *i2s_drvdata = snd_soc_dai_get_drvdata(dai);
	 if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		i2s_write_reg(i2s_drvdata->i2s_base, TXFFR, 1);
	else
		i2s_write_reg(i2s_drvdata->i2s_base, RXFFR, 1);

	return 0;
}

static int i2s_trigger(struct snd_pcm_substream *substream,
		int cmd, struct snd_soc_dai *dai)
{
	struct i2s_dev *i2s_drvdata = snd_soc_dai_get_drvdata(dai);
	int ret = 0;

	dev_dbg(i2s_drvdata->dev, "%s\n", __func__);
	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		i2s_drvdata->active++;
		i2s_start(i2s_drvdata, substream);
		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
			i2s_drvdata->playback_active = true;
		} else {
			i2s_drvdata->capture_active = true;
		}
		i2s_enable_cnt[i2s_drvdata->nid]++;
		break;
	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		i2s_drvdata->active--;
		i2s_stop(i2s_drvdata, substream);
		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
			i2s_drvdata->playback_active = false;
		} else {
			i2s_drvdata->capture_active = false;
		}
		i2s_enable_cnt[i2s_drvdata->nid]--;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	return ret;
}

static int i2s_set_fmt(struct snd_soc_dai *cpu_dai, unsigned int fmt)
{
	struct i2s_dev *dev = snd_soc_dai_get_drvdata(cpu_dai);
	int ret = 0;
	switch (fmt & SND_SOC_DAIFMT_CLOCK_PROVIDER_MASK) {
	case SND_SOC_DAIFMT_BC_FC:
		if (dev->capability & DW_I2S_SLAVE)
			ret = 0;
		else
			ret = -EINVAL;
		break;
	case SND_SOC_DAIFMT_BP_FP:
		if (dev->capability & DW_I2S_MASTER)
			ret = 0;
		else
			ret = -EINVAL;
		break;
	case SND_SOC_DAIFMT_BC_FP:
	case SND_SOC_DAIFMT_BP_FC:
		ret = -EINVAL;
		break;
	default:
		dev_dbg(dev->dev, "dwc : Invalid clock provider format\n");
		ret = -EINVAL;
		break;
	}
	return ret;
}

static int i2s_pcm_dai_probe(struct snd_soc_dai *dai)
{
	struct i2s_dev *i2s_drvdata = snd_soc_dai_get_drvdata(dai);

	snd_soc_dai_init_dma_data(dai, &i2s_drvdata->play_dma_data, &i2s_drvdata->capture_dma_data);
	return 0;
}

static const struct snd_soc_dai_ops i2s_dai_ops = {
	.probe		= i2s_pcm_dai_probe,
	.startup	= i2s_startup,
	.hw_params	= i2s_hw_params,
	.prepare	= i2s_prepare,
	.trigger	= i2s_trigger,
	.set_fmt	= i2s_set_fmt,
};

#ifdef CONFIG_PM
static int i2s_runtime_suspend(struct device *dev)
{
	struct i2s_dev *i2s_drvdata = dev_get_drvdata(dev);
	struct clk *clk = i2s_drvdata->mclk;

	dev_dbg(i2s_drvdata->dev, "%s\n", __func__);

	clk_disable(clk);

	return 0;
}

static int i2s_runtime_resume(struct device *dev)
{
	struct i2s_dev *i2s_drvdata = dev_get_drvdata(dev);
	struct clk *clk = i2s_drvdata->mclk;

	dev_dbg(i2s_drvdata->dev, "%s\n", __func__);
	clk_enable(clk);

	return 0;
}

static int i2s_suspend(struct snd_soc_component *component)
{
	struct i2s_dev *i2s_drvdata = snd_soc_component_get_drvdata(component);
	struct clk *clk = i2s_drvdata->mclk;

	dev_dbg(i2s_drvdata->dev, "%s\n", __func__);
	if(!pm_runtime_suspended(i2s_drvdata->dev)) {
		dev_dbg(i2s_drvdata->dev, "disable clk\n");
		clk_disable(clk);
	}

	return 0;
}

static int i2s_resume(struct snd_soc_component *component)
{
	struct i2s_dev *i2s_drvdata = snd_soc_component_get_drvdata(component);
	struct snd_soc_dai *dai = NULL;
	struct clk *clk = i2s_drvdata->mclk;
	int stream;

	dev_dbg(i2s_drvdata->dev, "%s\n", __func__);
	if(!pm_runtime_suspended(i2s_drvdata->dev)) {
		dev_dbg(i2s_drvdata->dev, "enable clk\n");
		clk_enable(clk);
		for_each_component_dais(component, dai) {
			for_each_pcm_streams(stream)
				if (snd_soc_dai_stream_active(dai, stream))
					i2s_config(i2s_drvdata, stream);
		}
	}

	return 0;
}
#else
#define i2s_suspend NULL
#define i2s_resume NULL
#endif

static int i2s_reset(struct platform_device *pdev, struct i2s_dev *i2s)
{
	struct reset_control *rst;
	struct reset_control *prst;
	struct reset_control *voprst;
	void __iomem *i2s0_io_base;
	int ret, reg_val;

	rst = devm_reset_control_get_optional_exclusive(&pdev->dev, "i2srst");
	if (IS_ERR(rst)) {
		return PTR_ERR(rst);
	}

	prst = devm_reset_control_get_optional_exclusive(&pdev->dev, "i2sprst");
	if (IS_ERR(prst)) {
		return PTR_ERR(prst);
	}

	voprst = devm_reset_control_get_optional_exclusive(&pdev->dev, "voprst");
	if (IS_ERR(prst)) {
		return PTR_ERR(prst);
	}

	ret = reset_control_assert(rst);
	WARN_ON(0 != ret);
	ret = reset_control_assert(prst);
	WARN_ON(0 != ret);
	ret = reset_control_deassert(rst);
	WARN_ON(0 != ret);
	ret = reset_control_deassert(prst);
	WARN_ON(0 != ret);
	ret = reset_control_deassert(voprst);
	WARN_ON(0 != ret);

	if (!of_property_read_bool(pdev->dev.of_node, "io_reuse_enable")) {
		i2s0_io_base = devm_ioremap(&pdev->dev, DIE0_I2S0_IO_ADDR, 12);
		if (!i2s0_io_base) {
			dev_err(&pdev->dev, "failed to remap i2s0 io ctl\n");
			return -ENOMEM;
		}

		/* set the i2s0 WCLK io to GPIO func */
		reg_val = readl((char *)i2s0_io_base);
		reg_val &= 0xfff8ffff;
		reg_val |= 0x20000;
		writel(reg_val, (char *)i2s0_io_base);

		/* set the i2s0 SDI io to GPIO func */
		reg_val = readl((char *)i2s0_io_base + 4);
		reg_val &= 0xfff8ffff;
		reg_val |= 0x20000;
		writel(reg_val, (char *)i2s0_io_base + 4);

		/* set the i2s0 SDO io to GPIO func */
		reg_val = readl((char *)i2s0_io_base + 8);
		reg_val &= 0xfff8ffff;
		reg_val |= 0x20000;
		writel(reg_val, (char *)i2s0_io_base + 8);
	}

	return 0;
}

static int i2s_open(struct snd_soc_component *component,
			      struct snd_pcm_substream *substream)
{
	struct i2s_dev *i2s_drvdata = snd_soc_component_get_drvdata(component);

	if ((substream->stream == SNDRV_PCM_STREAM_PLAYBACK && i2s_drvdata->playback_active == true) ||
		(substream->stream == SNDRV_PCM_STREAM_CAPTURE && i2s_drvdata->capture_active == true)) {
		dev_err(i2s_drvdata->dev, "i2s is busying\n");
		return -EBUSY;
	}

	return 0;
}

static const struct snd_soc_component_driver i2s_component = {
	.open         = i2s_open,
	.suspend      = i2s_suspend,
	.resume       = i2s_resume,
};

static const struct snd_soc_component_driver i2s_sof_component = {
	.open       = esw_sof_dma_open,
	.close      = esw_sof_dma_close,
	.hw_params  = esw_sof_dma_hw_params,
	.trigger    = esw_sof_dma_trigger,
};

static struct snd_soc_dai_driver i2s_dai[8] = {
	{
		.name = D0_HDMI_DAI_NAME,
		.id = 0,
		.ops = &i2s_dai_ops,
		.playback = {
			.stream_name = "Playback",
			.channels_min = MIN_CHANNEL_NUM,
			.channels_max = MAX_CHANNEL_NUM,
			.rates = ESW_I2S_RATES,
			.formats = ESW_I2S_FORMATS,
		},
	},
	{
		.name = "i2s0",
		.id = 1,
		.ops = &i2s_dai_ops,
		.playback = {
			.stream_name = "Playback",
			.channels_min = MIN_CHANNEL_NUM,
			.channels_max = MAX_CHANNEL_NUM,
			.rates = ESW_I2S_RATES,
			.formats = ESW_I2S_FORMATS,
		},
		.capture = {
			.stream_name = "Capture",
			.channels_min = MIN_CHANNEL_NUM,
			.channels_max = MAX_CHANNEL_NUM,
			.rates = ESW_I2S_RATES,
			.formats = ESW_I2S_FORMATS,
		},
	},
	{
		.name = "i2s1",
		.id = 0,
		.ops = &i2s_dai_ops,
		.playback = {
			.channels_min = MIN_CHANNEL_NUM,
			.channels_max = MAX_CHANNEL_NUM,
			.rates = ESW_I2S_RATES,
			.formats = ESW_I2S_FORMATS,
		},
		.capture = {
			.channels_min = MIN_CHANNEL_NUM,
			.channels_max = MAX_CHANNEL_NUM,
			.rates = ESW_I2S_RATES,
			.formats = ESW_I2S_FORMATS,
		},
	},
	{
		.name = "i2s2",
		.id = 0,
		.ops = &i2s_dai_ops,
		.playback = {
			.channels_min = MIN_CHANNEL_NUM,
			.channels_max = MAX_CHANNEL_NUM,
			.rates = ESW_I2S_RATES,
			.formats = ESW_I2S_FORMATS,
		},
		.capture = {
			.channels_min = MIN_CHANNEL_NUM,
			.channels_max = MAX_CHANNEL_NUM,
			.rates = ESW_I2S_RATES,
			.formats = ESW_I2S_FORMATS,
		},
	},
	{
		.name = D1_HDMI_DAI_NAME,
		.id = 0,
		.ops = &i2s_dai_ops,
		.playback = {
			.channels_min = MIN_CHANNEL_NUM,
			.channels_max = MAX_CHANNEL_NUM,
			.rates = ESW_I2S_RATES,
			.formats = ESW_I2S_FORMATS,
		},
	},
	{
		.name = "d1-i2s0",
		.id = 1,
		.ops = &i2s_dai_ops,
		.playback = {
			.channels_min = MIN_CHANNEL_NUM,
			.channels_max = MAX_CHANNEL_NUM,
			.rates = ESW_I2S_RATES,
			.formats = ESW_I2S_FORMATS,
		},
		.capture = {
			.channels_min = MIN_CHANNEL_NUM,
			.channels_max = MAX_CHANNEL_NUM,
			.rates = ESW_I2S_RATES,
			.formats = ESW_I2S_FORMATS,
		},
	},
	{
		.name = "d1-i2s1",
		.id = 0,
		.ops = &i2s_dai_ops,
		.playback = {
			.channels_min = MIN_CHANNEL_NUM,
			.channels_max = MAX_CHANNEL_NUM,
			.rates = ESW_I2S_RATES,
			.formats = ESW_I2S_FORMATS,
		},
		.capture = {
			.channels_min = MIN_CHANNEL_NUM,
			.channels_max = MAX_CHANNEL_NUM,
			.rates = ESW_I2S_RATES,
			.formats = ESW_I2S_FORMATS,
		},
	},
	{
		.name = "d1-i2s2",
		.id = 0,
		.ops = &i2s_dai_ops,
		.playback = {
			.channels_min = MIN_CHANNEL_NUM,
			.channels_max = MAX_CHANNEL_NUM,
			.rates = ESW_I2S_RATES,
			.formats = ESW_I2S_FORMATS,
		},
		.capture = {
			.channels_min = MIN_CHANNEL_NUM,
			.channels_max = MAX_CHANNEL_NUM,
			.rates = ESW_I2S_RATES,
			.formats = ESW_I2S_FORMATS,
		},
	},
};

static int i2s_sof_ringbuffer_init(struct i2s_dev *i2s_drvdata)
{
	struct resource res_sram;
	int ret;
	struct device_node *rb_node;
	u32 ringbuffer_in_addr;
	u32 ringbuffer_out_addr;

	rb_node = of_parse_phandle(i2s_drvdata->dev->of_node, "ringbuffer-region", 0);
	if (IS_ERR(rb_node)) {
		dev_err(i2s_drvdata->dev, "Get phandle dsp-sram error\n");
		return -ENODEV;
	}
	of_node_put(i2s_drvdata->dev->of_node);

	ret = of_address_to_resource(rb_node, 0, &res_sram);
	if (ret) {
		dev_err(i2s_drvdata->dev, "failed to get ring buffer address\n");
		return ret;
	}
	of_node_put(rb_node);

	ringbuffer_in_addr = res_sram.start + DSP_RB_COMP_SIZE + DSP_RB_PCM_P_SZIE;
	ringbuffer_out_addr = res_sram.start + DSP_RB_COMP_SIZE;

	i2s_drvdata->pos[0] =  devm_ioremap(i2s_drvdata->dev, ringbuffer_out_addr, DSP_RB_PCM_C_SZIE + DSP_RB_PCM_P_SZIE);
	if(!i2s_drvdata->pos[0]) {
		dev_err(i2s_drvdata->dev, "failed to remap stream out pos\n");
		return -ENOMEM;
	}
	i2s_drvdata->pos[1] = i2s_drvdata->pos[0] + DSP_RB_PCM_P_SZIE;

	memset(i2s_drvdata->pos[0], 0, (DSP_RB_PCM_P_SZIE + DSP_RB_PCM_C_SZIE));

	i2s_drvdata->rb_out_iova = dma_map_page_attrs(i2s_drvdata->chan[0]->device->dev,
										   phys_to_page(ringbuffer_out_addr + DSP_RB_POS_SZIE),
										   offset_in_page(ringbuffer_out_addr + DSP_RB_POS_SZIE),
										   DSP_RB_DATA_SZIE, DMA_BIDIRECTIONAL, DMA_ATTR_SKIP_CPU_SYNC);
	if (dma_mapping_error(i2s_drvdata->chan[0]->device->dev, i2s_drvdata->rb_out_iova)) {
		dev_err(i2s_drvdata->dev, "dma mapping failed\n");
		return -ENOMEM;
	}
	dev_dbg(i2s_drvdata->dev, "rb out iova_addr:0x%llx\n", i2s_drvdata->rb_out_iova);

	i2s_drvdata->rb_in_iova = dma_map_page_attrs(i2s_drvdata->chan[1]->device->dev,
										  phys_to_page(ringbuffer_in_addr + DSP_RB_POS_SZIE),
										  offset_in_page(ringbuffer_in_addr + DSP_RB_POS_SZIE),
										  DSP_RB_DATA_SZIE, DMA_BIDIRECTIONAL, DMA_ATTR_SKIP_CPU_SYNC);
	if (dma_mapping_error(i2s_drvdata->chan[1]->device->dev, i2s_drvdata->rb_in_iova)) {
		dev_err(i2s_drvdata->dev, "dma mapping failed\n");
		dma_unmap_page_attrs(i2s_drvdata->chan[0]->device->dev, i2s_drvdata->rb_out_iova, DSP_RB_DATA_SZIE,
							 DMA_BIDIRECTIONAL, DMA_ATTR_SKIP_CPU_SYNC);
		return -ENOMEM;
	}
	dev_dbg(i2s_drvdata->dev, "rb in iova_addr:0x%llx\n", i2s_drvdata->rb_in_iova);

	return 0;
}

static int i2s_probe(struct platform_device *pdev)
{
	struct i2s_dev *i2s_drvdata;
	struct resource *res;
	int ret = 0;
	int nid = 0;
	unsigned long vo_top_csr;
	int dai_offset = 0;

	dev_info(&pdev->dev, "dev name:%s\n", pdev->dev.of_node->name);
	i2s_drvdata = devm_kzalloc(&pdev->dev, sizeof(*i2s_drvdata), GFP_KERNEL);
	if (!i2s_drvdata)
		return -ENOMEM;

	res = platform_get_resource(pdev,IORESOURCE_MEM, 0);
	i2s_drvdata->i2s_base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(i2s_drvdata->i2s_base)) {
		dev_err(&pdev->dev, "devm_ioremap_resource failed\n");
		return PTR_ERR(i2s_drvdata->i2s_base);
	}
	i2s_drvdata->dev = &pdev->dev;

	nid = i2s_get_nid(&pdev->dev);
	if (nid == 0) {
		dai_offset = DIE0_DAI_DRIVER_OFFSET;
	} else if (nid == 1) {
		dai_offset = DIE1_DAI_DRIVER_OFFSET;
	} else {
		dev_err(&pdev->dev, "%s:%d, NUMA_NO_NODE\n", __func__, __LINE__);
		return -EFAULT;
	}
	i2s_drvdata->nid = nid;

	ret = set_mclk(&pdev->dev, i2s_drvdata);
	if (ret < 0) {
		return ret;
	}

	if (of_node_name_prefix(pdev->dev.of_node, "i2s0")) {
		ret = i2s_reset(pdev, i2s_drvdata);
		if (ret != 0) {
			dev_err(&pdev->dev, "i2s_reset failed\n");
			goto err_probe;
		}
	}

	dev_set_drvdata(&pdev->dev, i2s_drvdata);

	vo_top_csr = get_vo_top_csr(nid);
	if (of_node_name_prefix(pdev->dev.of_node, "i2s0")) {
		i2s_drvdata->i2s_div_base = devm_ioremap(i2s_drvdata->dev, vo_top_csr + VO_I2S0_DIV_NUM, 4);
		if (!i2s_drvdata->i2s_div_base) {
			dev_err(&pdev->dev, "failed to remap i2s0 div config\n");
			ret = -ENOMEM;
			goto err_probe;
		}
		if (of_property_read_bool(pdev->dev.of_node, "dsp_sof_enable")) {
			ret = devm_snd_soc_register_component(&pdev->dev, &i2s_sof_component, &i2s_dai[dai_offset], 2);
		} else {
			ret = devm_snd_soc_register_component(&pdev->dev, &i2s_component, &i2s_dai[dai_offset], 2);
		}
	} else if (of_node_name_prefix(pdev->dev.of_node, "i2s1")) {
		i2s_drvdata->i2s_div_base = devm_ioremap(i2s_drvdata->dev, vo_top_csr + VO_I2S1_DIV_NUM, 4);
		if (!i2s_drvdata->i2s_div_base) {
			dev_err(&pdev->dev, "failed to remap i2s1 div config\n");
			ret = -ENOMEM;
			goto err_probe;
		}
		if (of_property_read_bool(pdev->dev.of_node, "dsp_sof_enable")) {
			ret = devm_snd_soc_register_component(&pdev->dev, &i2s_sof_component, &i2s_dai[2 + dai_offset], 1);
		} else {
			ret = devm_snd_soc_register_component(&pdev->dev, &i2s_component, &i2s_dai[2 + dai_offset], 1);
		}
	} else {
		i2s_drvdata->i2s_div_base = devm_ioremap(i2s_drvdata->dev, vo_top_csr + VO_I2S2_DIV_NUM, 4);
		if (!i2s_drvdata->i2s_div_base) {
			dev_err(&pdev->dev, "failed to remap i2s2 div config\n");
			ret = -ENOMEM;
			goto err_probe;
		}
		if (of_property_read_bool(pdev->dev.of_node, "dsp_sof_enable")) {
			ret = devm_snd_soc_register_component(&pdev->dev, &i2s_sof_component, &i2s_dai[3 + dai_offset], 1);
		} else {
			ret = devm_snd_soc_register_component(&pdev->dev, &i2s_component, &i2s_dai[3 + dai_offset], 1);
		}
	}
	if (ret != 0) {
		dev_err(&pdev->dev, "not able to register dai\n");
		goto err_probe;
	}

	i2s_drvdata->use_pio = false;

	if (of_property_read_bool(pdev->dev.of_node, "dsp_sof_enable")) {
		ret = esw_sof_dma_init(i2s_drvdata);
		if (ret != 0) {
			dev_err(&pdev->dev, "pcm dma init failed\n");
			goto err_probe;
		}
		ret = i2s_sof_ringbuffer_init(i2s_drvdata);
		if (ret != 0) {
			dev_err(&pdev->dev, "pcm ringbuffer init failed\n");
			goto err_probe;
		}
	} else {
		ret = esw_pcm_dma_dai_register(i2s_drvdata);
		if (ret) {
			dev_err(&pdev->dev, "could not register pcm: %d\n", ret);
			goto err_probe;
		}
	}

	i2s_drvdata->i2s_reg_comp1 = I2S_COMP_PARAM_1;
	i2s_drvdata->i2s_reg_comp2 = I2S_COMP_PARAM_2;
	ret = i2s_configure_res_by_dt(i2s_drvdata, res);
	if (ret < 0) {
		dev_err(&pdev->dev, "i2s_configure_res_by_dt failed\n");
		goto err_probe;
	}

	pm_runtime_enable(&pdev->dev);

	audio_proc_module_init();

#ifdef CONFIG_PM
	clk_disable(i2s_drvdata->mclk);
#endif
	return 0;
err_probe:
	clk_disable_unprepare(i2s_drvdata->mclk);

	return ret;
}

static int i2s_remove(struct platform_device *pdev)
{
	struct i2s_dev *i2s_drvdata = dev_get_drvdata(&pdev->dev);

	dev_info(&pdev->dev, "dev name:%s\n", pdev->dev.of_node->name);

	clk_disable_unprepare(i2s_drvdata->mclk);
	pm_runtime_disable(&pdev->dev);
	audio_proc_module_exit();

	if (of_property_read_bool(pdev->dev.of_node, "dsp_sof_enable")) {
		dma_unmap_page_attrs(i2s_drvdata->chan[0]->device->dev, i2s_drvdata->rb_out_iova, DSP_RB_DATA_SZIE,
							DMA_BIDIRECTIONAL, DMA_ATTR_SKIP_CPU_SYNC);
		dma_unmap_page_attrs(i2s_drvdata->chan[1]->device->dev, i2s_drvdata->rb_in_iova, DSP_RB_DATA_SZIE,
							DMA_BIDIRECTIONAL, DMA_ATTR_SKIP_CPU_SYNC);
	} else {
		kfree(i2s_drvdata->conv_buf[0]);
		kfree(i2s_drvdata->conv_buf[1]);
	}

	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id i2s_of_match[] = {
	{ .compatible = "snps,i2s", },
	{},
};

MODULE_DEVICE_TABLE(of, i2s_of_match);
#endif

static const struct dev_pm_ops i2s_pm_ops = {
	SET_RUNTIME_PM_OPS(i2s_runtime_suspend, i2s_runtime_resume, NULL)
};

static struct platform_driver i2s_driver = {
	.probe		= i2s_probe,
	.remove		= i2s_remove,
	.driver		= {
		.name	= "es-i2s",
		.of_match_table = of_match_ptr(i2s_of_match),
		.pm = &i2s_pm_ops,
	},
};

module_platform_driver(i2s_driver);

MODULE_AUTHOR("ESWIN, INC.");
MODULE_DESCRIPTION("I2S driver");
MODULE_LICENSE("GPL");
