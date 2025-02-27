/* SPDX-License-Identifier: GPL-2.0
 ****************************************************************************
 *
 *    The MIT License (MIT)
 *
 *    Copyright 2019 Verisilicon(Beijing) Co.,Ltd. All Rights Reserved.
 *
 *    Permission is hereby granted, free of charge, to any person obtaining a
 *    copy of this software and associated documentation files (the "Software"),
 *    to deal in the Software without restriction, including without limitation
 *    the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *    and/or sell copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following conditions:
 *
 *    The above copyright notice and this permission notice shall be included in
 *    all copies or substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *    DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************
 *
 *    The GPL License (GPL)
 *
 *    Copyright 2019 Verisilicon(Beijing) Co.,Ltd. All Rights Reserved.
 *
 *    This program is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU General Public License
 *    as published by the Free Software Foundation; either version 2
 *    of the License, or (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software Foundation,
 *    Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *****************************************************************************
 *
 *    Note: This software is released under dual MIT and GPL licenses. A
 *    recipient may use this file under the terms of either the MIT license or
 *    GPL License. If you wish to use only one license not the other, you can
 *    indicate your decision by deleting one of the above license notices in your
 *    version of this file.
 *
 *****************************************************************************
 */

    VCMDREG(HWIF_VCMD_HW_ID,                          0,  0xffff0000,       16,        0, RO, "HW ID"),
    VCMDREG(HWIF_VCMD_HW_VERSION,                     0,  0x0000ffff,        0,        0, RO, "version of hw(1.0.0).[15:12]-major [11:8]-minor [7:0]-build"),
    VCMDREG(HWIF_VCMD_HW_BUILD_DATE,                  4,  0xffffffff,        0,        0, RO, "Hw package generation date in BCD code"),
    VCMDREG(HWIF_VCMD_EXT_ABN_INT_SRC_VCD_MMU,        8,  0x08000000,       27,        0, RO, "external abnormal interrupt source from mmu of vcd."),
    VCMDREG(HWIF_VCMD_EXT_ABN_INT_SRC_VCD_L2CACHE,    8,  0x04000000,       26,        0, RO, "external abnormal interrupt source from l2cache of vcd."),
    VCMDREG(HWIF_VCMD_EXT_ABN_INT_SRC_VCD_DEC400,     8,  0x02000000,       25,        0, RO, "external abnormal interrupt source from dec400 of vcd."),
    VCMDREG(HWIF_VCMD_EXT_ABN_INT_SRC_VCD,            8,  0x01000000,       24,        0, RO, "external abnormal interrupt source from vcd."),
    VCMDREG(HWIF_VCMD_EXT_ABN_INT_SRC_CUTREE_MMU,     8,  0x00200000,       21,        0, RO, "external abnormal interrupt source from mmu of cutree."),
    VCMDREG(HWIF_VCMD_EXT_ABN_INT_SRC_VCE_MMU,        8,  0x00100000,       20,        0, RO, "external abnormal interrupt source from mmu of vce."),
    VCMDREG(HWIF_VCMD_EXT_ABN_INT_SRC_VCE_L2CACHE,    8,  0x00080000,       19,        0, RO, "external abnormal interrupt source from l2 cache of vce."),
    VCMDREG(HWIF_VCMD_EXT_ABN_INT_SRC_VCE_DEC400,     8,  0x00040000,       18,        0, RO, "external abnormal interrupt source from dec400 of vce."),
    VCMDREG(HWIF_VCMD_EXT_ABN_INT_SRC_CUTREE,         8,  0x00020000,       17,        0, RO, "external abnormal interrupt source from cutree."),
    VCMDREG(HWIF_VCMD_EXT_ABN_INT_SRC_VCE,            8,  0x00010000,       16,        0, RO, "external abnormal interrupt source from vce."),
    VCMDREG(HWIF_VCMD_EXT_NORM_INT_SRC_VCD_MMU,       8,  0x00000800,       11,        0, RO, "external normal interrupt source from mmu of vcd."),
    VCMDREG(HWIF_VCMD_EXT_NORM_INT_SRC_VCD_L2CACHE,   8,  0x00000400,       10,        0, RO, "external normal interrupt source from l2cache of vcd."),
    VCMDREG(HWIF_VCMD_EXT_NORM_INT_SRC_VCD_DEC400,    8,  0x00000200,        9,        0, RO, "external normal interrupt source from dec400 of vcd."),
    VCMDREG(HWIF_VCMD_EXT_NORM_INT_SRC_VCD,           8,  0x00000100,        8,        0, RO, "external normal interrupt source from vcd."),
    VCMDREG(HWIF_VCMD_EXT_NORM_INT_SRC_CUTREE_MMU,    8,  0x00000020,        5,        0, RO, "external normal interrupt source from mmu of cutree."),
    VCMDREG(HWIF_VCMD_EXT_NORM_INT_SRC_VCE_MMU,       8,  0x00000010,        4,        0, RO, "external normal interrupt source from mmu of vce."),
    VCMDREG(HWIF_VCMD_EXT_NORM_INT_SRC_VCE_L2CACHE,   8,  0x00000008,        3,        0, RO, "external normal interrupt source from l2 cache of vce."),
    VCMDREG(HWIF_VCMD_EXT_NORM_INT_SRC_VCE_DEC400,    8,  0x00000004,        2,        0, RO, "external normal interrupt source from dec400 of vce."),
    VCMDREG(HWIF_VCMD_EXT_NORM_INT_SRC_CUTREE,        8,  0x00000002,        1,        0, RO, "external normal interrupt source from cutree."),
    VCMDREG(HWIF_VCMD_EXT_NORM_INT_SRC_VCE,           8,  0x00000001,        0,        0, RO, "external normal interrupt source from vce."),
    VCMDREG(HWIF_VCMD_EXE_CMDBUF_COUNT,               12, 0xffffffff,        0,        0, RO, "Hw increases this counter by 1 after one more command buffer has been executed"),
    VCMDREG(HWIF_VCMD_EXECUTING_CMD,                  16, 0xffffffff,        0,        0, RO, "the first 32 bits of the executing cmd."),
    VCMDREG(HWIF_VCMD_EXECUTING_CMD_MSB,              20, 0xffffffff,        0,        0, RO, "the second 32 bits of the executing cmd."),
    VCMDREG(HWIF_VCMD_AXI_TOTAL_AR_LEN,               24, 0xffffffff,        0,        0, RO, "axi total ar length"),
    VCMDREG(HWIF_VCMD_AXI_TOTAL_R,                    28, 0xffffffff,        0,        0, RO, "axi total r"),
    VCMDREG(HWIF_VCMD_AXI_TOTAL_AR,                   32, 0xffffffff,        0,        0, RO, "axi total ar"),
    VCMDREG(HWIF_VCMD_AXI_TOTAL_R_LAST,               36, 0xffffffff,        0,        0, RO, "axi total r last"),
    VCMDREG(HWIF_VCMD_AXI_TOTAL_AW_LEN,               40, 0xffffffff,        0,        0, RO, "axi total aw length"),
    VCMDREG(HWIF_VCMD_AXI_TOTAL_W,                    44, 0xffffffff,        0,        0, RO, "axi total w"),
    VCMDREG(HWIF_VCMD_AXI_TOTAL_AW,                   48, 0xffffffff,        0,        0, RO, "axi total aw"),
    VCMDREG(HWIF_VCMD_AXI_TOTAL_W_LAST,               52, 0xffffffff,        0,        0, RO, "axi total w last"),
    VCMDREG(HWIF_VCMD_AXI_TOTAL_B,                    56, 0xffffffff,        0,        0, RO, "axi total b"),
    VCMDREG(HWIF_VCMD_AXI_AR_VALID,                   60, 0x80000000,       31,        0, RO, "axi ar valid"),
    VCMDREG(HWIF_VCMD_AXI_AR_READY,                   60, 0x40000000,       30,        0, RO, "axi ar ready"),
    VCMDREG(HWIF_VCMD_AXI_R_VALID,                    60, 0x20000000,       29,        0, RO, "axi r valid"),
    VCMDREG(HWIF_VCMD_AXI_R_READY,                    60, 0x10000000,       28,        0, RO, "axi r ready"),
    VCMDREG(HWIF_VCMD_AXI_AW_VALID,                   60, 0x08000000,       27,        0, RO, "axi aw valid"),
    VCMDREG(HWIF_VCMD_AXI_AW_READY,                   60, 0x04000000,       26,        0, RO, "axi aw ready"),
    VCMDREG(HWIF_VCMD_AXI_W_VALID,                    60, 0x02000000,       25,        0, RO, "axi w valid"),
    VCMDREG(HWIF_VCMD_AXI_W_READY,                    60, 0x01000000,       24,        0, RO, "axi w ready"),
    VCMDREG(HWIF_VCMD_AXI_B_VALID,                    60, 0x00800000,       23,        0, RO, "axi b valid"),
    VCMDREG(HWIF_VCMD_AXI_B_READY,                    60, 0x00400000,       22,        0, RO, "axi b ready"),
    VCMDREG(HWIF_VCMD_WORK_STATE,                     60, 0x00000007,        0,        0, RO, "hw work state. 0-IDLE 1-WORK 2-STALL 3-PEND 4-ABORT"),
    VCMDREG(HWIF_VCMD_INIT_MODE,                      64, 0x00000080,        7,        0, RW, "After executed a END command in init mode, VCMD will get back to normal mode"),
    VCMDREG(HWIF_VCMD_AXI_CLK_GATE_DISABLE,           64, 0x00000040,        6,        0, RW, "keep axi_clk always on when this bit is set to 1"),
    VCMDREG(HWIF_VCMD_MASTER_OUT_CLK_GATE_DISABLE,    64, 0x00000020,        5,        0, RW, "keep master_out_clk(APB/AHB master) always on when this bit is set to 1"),
    VCMDREG(HWIF_VCMD_CORE_CLK_GATE_DISABLE,          64, 0x00000010,        4,        0, RW, "keep core_clk always on when this bit is set to 1"),
    VCMDREG(HWIF_VCMD_ABORT_MODE,                     64, 0x00000008,        3,        0, RW, "0:abort after finishing current cmdbuf command.1:abort immediately "),
    VCMDREG(HWIF_VCMD_RESET_CORE,                     64, 0x00000004,        2,        0, RW, "sw write 1 to this bit will rset HW core logic when AXI/APB bus is idle."),
    VCMDREG(HWIF_VCMD_RESET_ALL,                      64, 0x00000002,        1,        0, RW, "sw write 1 to this bit will rset HW immediately including all swregs and AXI/APB  bus logic"),
    VCMDREG(HWIF_VCMD_START_TRIGGER,                  64, 0x00000001,        0,        0, RW, "0:abort previou task and stop hw. 1:trigger hw to fetch and execute commands."),
    VCMDREG(HWIF_VCMD_IRQ_INTCMD,                     68, 0xffff0000,       16,        0, RW, "interrupt sources which are triggered by command buffer id.. Only for version 1.0.c"),
    VCMDREG(HWIF_VCMD_IRQ_JMPP,                       68, 0x00000080,        7,        0, RW, "interrupt source which is triggered by JMP command when hw goes to PEND state."),
    VCMDREG(HWIF_VCMD_IRQ_JMPD,                       68, 0x00000040,        6,        0, RW, "interrupt source which is triggered by JMP command directly."),
    VCMDREG(HWIF_VCMD_IRQ_RESET,                      68, 0x00000020,        5,        0, RW, "interrupt source which is triggered by hw reset or sw_vcmd_reset_all."),
    VCMDREG(HWIF_VCMD_IRQ_ABORT,                      68, 0x00000010,        4,        0, RW, "interrupt source which is triggered by abort operation."),
    VCMDREG(HWIF_VCMD_IRQ_CMDERR,                     68, 0x00000008,        3,        0, RW, "interrupt source which is triggered when there is illegal command in cmdbuf"),
    VCMDREG(HWIF_VCMD_IRQ_TIMEOUT,                    68, 0x00000004,        2,        0, RW, "interrupt source which is triggered when vcmd timeout."),
    VCMDREG(HWIF_VCMD_IRQ_BUSERR,                     68, 0x00000002,        1,        0, RW, "interrupt source which is triggered when there is bus error."),
    VCMDREG(HWIF_VCMD_IRQ_ENDCMD,                     68, 0x00000001,        0,        0, RW, "interrupt source which is triggered by END command."),
    VCMDREG(HWIF_VCMD_IRQ_INTCMD_EN,                  72, 0xffff0000,       16,        0, RW, "interrupt sources which are triggered by command buffer id. Only for version 1.0.c"),
    VCMDREG(HWIF_VCMD_IRQ_JMPP_EN,                    72, 0x00000080,        7,        0, RW, "interrupt enable for sw_vcmd_irq_jmpp"),
    VCMDREG(HWIF_VCMD_IRQ_JMPD_EN,                    72, 0x00000040,        6,        0, RW, "interrupt enable for sw_vcmd_irq_jmpd"),
    VCMDREG(HWIF_VCMD_IRQ_RESET_EN,                   72, 0x00000020,        5,        0, RW, "interrupt enable for sw_vcmd_irq_reset"),
    VCMDREG(HWIF_VCMD_IRQ_ABORT_EN,                   72, 0x00000010,        4,        0, RW, "interrupt enable for sw_vcmd_irq_abort"),
    VCMDREG(HWIF_VCMD_IRQ_CMDERR_EN,                  72, 0x00000008,        3,        0, RW, "interrupt enable for sw_vcmd_irq_cmderr"),
    VCMDREG(HWIF_VCMD_IRQ_TIMEOUT_EN,                 72, 0x00000004,        2,        0, RW, "interrupt enable for sw_vcmd_irq_timeout"),
    VCMDREG(HWIF_VCMD_IRQ_BUSERR_EN,                  72, 0x00000002,        1,        0, RW, "interrupt enable for sw_vcmd_irq_buserr"),
    VCMDREG(HWIF_VCMD_IRQ_ENDCMD_EN,                  72, 0x00000001,        0,        0, RW, "interrupt enable for sw_vcmd_irq_endcmd"),
    VCMDREG(HWIF_VCMD_TIMEOUT_EN,                     76, 0x80000000,       31,        0, RW, "1:timeout work. 0: timeout do not work"),
    VCMDREG(HWIF_VCMD_TIMEOUT_CYCLES,                 76, 0x7fffffff,        0,        0, RW, "sw_vcmd_irq_timeout will be generated when timeout counter is equal to this value."),
    VCMDREG(HWIF_VCMD_EXECUTING_CMD_ADDR,             80, 0xffffffff,        0,        0, RW, "the least 32 bits address of the executing command"),
    VCMDREG(HWIF_VCMD_EXECUTING_CMD_ADDR_MSB,         84, 0xffffffff,        0,        0, RW, "the most  32 bits address of the executing command"),
    VCMDREG(HWIF_VCMD_EXE_CMDBUF_LENGTH,              88, 0x0000ffff,        0,        0, RW, "the length of current command buffer in unit of 64bits."),
    VCMDREG(HWIF_VCMD_CMD_SWAP,                       92, 0xf0000000,       28,        0, RW, "axi data swapping"),
    VCMDREG(HWIF_VCMD_MAX_BURST_LEN,                  92, 0x00ff0000,       16,        0, RW, "max burst length which will be sent to axi bus"),
    VCMDREG(HWIF_VCMD_AXI_ID_RD,                      92, 0x0000ff00,        8,        0, RW, "the arid which will be used on axi bus reading"),
    VCMDREG(HWIF_VCMD_AXI_ID_WR,                      92, 0x000000ff,        0,        0, RW, "the awid which will be used on axi bus writing"),
    VCMDREG(HWIF_VCMD_RDY_CMDBUF_COUNT,               96, 0xffffffff,        0,        0, RW, "sw increases this counter by 1 after one more command buffer was ready."),
    VCMDREG(HWIF_VCMD_EXT_ABN_INT_SRC_VCD_MMU_GATE,   100, 0x10000000,       28,        0, RW, "external abnormal interrupt source from mmu of vcd gate."),
    VCMDREG(HWIF_VCMD_EXT_ABN_INT_SRC_VCD_L2CACHE_GATE, 100, 0x08000000,       27,        0, RW, "external abnormal interrupt source from l2cache of vcd gate."),
    VCMDREG(HWIF_VCMD_EXT_ABN_INT_SRC_VCD_DEC400_GATE,  100, 0x04000000,       26,        0, RW, "external abnormal interrupt source from dec400 of vcd gate."),
    VCMDREG(HWIF_VCMD_EXT_ABN_INT_SRC_VCD_GATE,         100, 0x01000000,       24,        0, RW, "external abnormal interrupt source from vcd gate."),
    VCMDREG(HWIF_VCMD_EXT_ABN_INT_SRC_CUTREE_MMU_GATE,  100, 0x00200000,       21,        0, RW, "external abnormal interrupt source from mmu of cutree gate."),
    VCMDREG(HWIF_VCMD_EXT_ABN_INT_SRC_VCE_MMU_GATE,     100, 0x00100000,       20,        0, RW, "external abnormal interrupt source from mmu of vce gate."),
    VCMDREG(HWIF_VCMD_EXT_ABN_INT_SRC_VCE_L2CACHE_GATE, 100, 0x00080000,       19,        0, RW, "external abnormal interrupt source from l2 cache of vce gate."),
    VCMDREG(HWIF_VCMD_EXT_ABN_INT_SRC_VCE_DEC400_GATE,  100, 0x00040000,       18,        0, RW, "external abnormal interrupt source from dec400 of vce gate."),
    VCMDREG(HWIF_VCMD_EXT_ABN_INT_SRC_CUTREE_GATE,      100, 0x00020000,       17,        0, RW, "external abnormal interrupt source from cutree gate."),
    VCMDREG(HWIF_VCMD_EXT_ABN_INT_SRC_VCE_GATE,         100, 0x00010000,       16,        0, RW, "external abnormal interrupt source from vce gate."),
    VCMDREG(HWIF_VCMD_EXT_NORM_INT_SRC_VCD_MMU_GATE,    100, 0x00000800,       11,        0, RW, "external normal interrupt source from mmu of vcd gate."),
    VCMDREG(HWIF_VCMD_EXT_NORM_INT_SRC_VCD_L2CACHE_GATE, 100, 0x00000400,       10,        0, RW, "external normal interrupt source from l2cache of vcd gate."),
    VCMDREG(HWIF_VCMD_EXT_NORM_INT_SRC_VCD_DEC400_GATE,  100, 0x00000200,        9,        0, RW, "external normal interrupt source from dec400 of vcd gate."),
    VCMDREG(HWIF_VCMD_EXT_NORM_INT_SRC_VCD_GATE,         100, 0x00000100,        8,        0, RW, "external normal interrupt source from vcd gate."),
    VCMDREG(HWIF_VCMD_EXT_NORM_INT_SRC_CUTREE_MMU_GATE,  100, 0x00000020,        5,        0, RW, "external normal interrupt source from mmu of cutree gate."),
    VCMDREG(HWIF_VCMD_EXT_NORM_INT_SRC_VCE_MMU_GATE,     100, 0x00000010,        4,        0, RW, "external normal interrupt source from mmu of vce gate."),
    VCMDREG(HWIF_VCMD_EXT_NORM_INT_SRC_VCE_L2CACHE_GATE, 100, 0x00000008,        3,        0, RW, "external normal interrupt source from l2 cache of vce gate."),
    VCMDREG(HWIF_VCMD_EXT_NORM_INT_SRC_VCE_DEC400_GATE,  100, 0x00000004,        2,        0, RW, "external normal interrupt source from dec400 of vce gate."),
    VCMDREG(HWIF_VCMD_EXT_NORM_INT_SRC_CUTREE_GATE,      100, 0x00000002,        1,        0, RW, "external normal interrupt source from cutree gate."),
    VCMDREG(HWIF_VCMD_EXT_NORM_INT_SRC_VCE_GATE,         100, 0x00000001,        0,        0, RW, "external normal interrupt source from vce gate."),
    VCMDREG(HWIF_VCMD_CMDBUF_EXECUTING_ID,               104, 0xffffffff,        0,        0, RW, "The ID of current executing command buffer.used after version 1.1.2."),
