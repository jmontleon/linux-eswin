/*************************************************************************/ /*!
@Title          Hardware definition file rgxmhdefs_km.h
@Brief          The file contains auto-generated hardware definitions without
                BVNC-specific compile time conditionals.
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
@License        Dual MIT/GPLv2

The contents of this file are subject to the MIT license as set out below.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public License Version 2 ("GPL") in which case the provisions
of GPL are applicable instead of those above.

If you wish to allow use of your version of this file only under the terms of
GPL, and not to allow others to use your version of this file under the terms
of the MIT license, indicate your decision by deleting the provisions above
and replace them with the notice and other provisions required by GPL as set
out in the file called "GPL-COPYING" included in this distribution. If you do
not delete the provisions above, a recipient may use your version of this file
under the terms of either the MIT license or GPL.

This License is also included in this distribution in the file called
"MIT-COPYING".

EXCEPT AS OTHERWISE STATED IN A NEGOTIATED AGREEMENT: (A) THE SOFTWARE IS
PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT; AND (B) IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/ /**************************************************************************/

/*               ****   Autogenerated C -- do not edit    ****               */

/*
 *      rogue_mh.def
 */


#ifndef RGXMHDEFS_KM_H
#define RGXMHDEFS_KM_H

#include "img_types.h"
#include "img_defs.h"


#define RGXMHDEFS_KM_REVISION 0

#define RGX_MH_TAG_SB_TDM_CTL_ENCODING_TDM_CTL_TAG_FENCE                            (0x00000000U)
#define RGX_MH_TAG_SB_TDM_CTL_ENCODING_TDM_CTL_TAG_CONTEXT                          (0x00000001U)
#define RGX_MH_TAG_SB_TDM_CTL_ENCODING_TDM_CTL_TAG_QUEUE                            (0x00000002U)


#define RGX_MH_TAG_SB_TDM_DMA_ENCODING_TDM_DMA_TAG_CTL_STREAM                       (0x00000000U)
#define RGX_MH_TAG_SB_TDM_DMA_ENCODING_TDM_DMA_TAG_CTX_BUFFER                       (0x00000001U)
#define RGX_MH_TAG_SB_TDM_DMA_ENCODING_TDM_DMA_TAG_QUEUE_CTL                        (0x00000002U)


#define RGX_MH_TAG_SB_PMD_ENCODING_PM_TAG_PMD_TAFSTACK                              (0x00000008U)
#define RGX_MH_TAG_SB_PMD_ENCODING_PM_TAG_PMD_TAMLIST                               (0x00000009U)
#define RGX_MH_TAG_SB_PMD_ENCODING_PM_TAG_PMD_3DFSTACK                              (0x0000000aU)
#define RGX_MH_TAG_SB_PMD_ENCODING_PM_TAG_PMD_3DMLIST                               (0x0000000bU)
#define RGX_MH_TAG_SB_PMD_ENCODING_PM_TAG_PMD_PMCTX0                                (0x0000000cU)
#define RGX_MH_TAG_SB_PMD_ENCODING_PM_TAG_PMD_PMCTX1                                (0x0000002dU)
#define RGX_MH_TAG_SB_PMD_ENCODING_PM_TAG_PMD_UFSTACK                               (0x0000000fU)
#define RGX_MH_TAG_SB_PMD_ENCODING_PM_TAG_PMD_TAMMUSTACK                            (0x00000012U)
#define RGX_MH_TAG_SB_PMD_ENCODING_PM_TAG_PMD_3DMMUSTACK                            (0x00000013U)
#define RGX_MH_TAG_SB_PMD_ENCODING_PM_TAG_PMD_TAUFSTACK                             (0x00000016U)
#define RGX_MH_TAG_SB_PMD_ENCODING_PM_TAG_PMD_3DUFSTACK                             (0x00000017U)
#define RGX_MH_TAG_SB_PMD_ENCODING_PM_TAG_PMD_3DVFP                                 (0x00000019U)
#define RGX_MH_TAG_SB_PMD_ENCODING_PM_TAG_PMD_TAVFP                                 (0x0000001aU)


#define RGX_MH_TAG_SB_PMA_ENCODING_PM_TAG_PMA_TAFSTACK                              (0x00000000U)
#define RGX_MH_TAG_SB_PMA_ENCODING_PM_TAG_PMA_TAMLIST                               (0x00000001U)
#define RGX_MH_TAG_SB_PMA_ENCODING_PM_TAG_PMA_3DFSTACK                              (0x00000002U)
#define RGX_MH_TAG_SB_PMA_ENCODING_PM_TAG_PMA_3DMLIST                               (0x00000003U)
#define RGX_MH_TAG_SB_PMA_ENCODING_PM_TAG_PMA_PMCTX0                                (0x00000004U)
#define RGX_MH_TAG_SB_PMA_ENCODING_PM_TAG_PMA_PMCTX1                                (0x00000025U)
#define RGX_MH_TAG_SB_PMA_ENCODING_PM_TAG_PMA_MAVP                                  (0x00000006U)
#define RGX_MH_TAG_SB_PMA_ENCODING_PM_TAG_PMA_UFSTACK                               (0x00000007U)
#define RGX_MH_TAG_SB_PMA_ENCODING_PM_TAG_PMA_TAMMUSTACK                            (0x00000008U)
#define RGX_MH_TAG_SB_PMA_ENCODING_PM_TAG_PMA_3DMMUSTACK                            (0x00000009U)
#define RGX_MH_TAG_SB_PMA_ENCODING_PM_TAG_PMA_TAUFSTACK                             (0x00000014U)
#define RGX_MH_TAG_SB_PMA_ENCODING_PM_TAG_PMA_3DUFSTACK                             (0x00000015U)
#define RGX_MH_TAG_SB_PMA_ENCODING_PM_TAG_PMA_TAVFP                                 (0x00000018U)


#define RGX_MH_TAG_SB_TA_ENCODING_TA_TAG_PPP                                        (0x00000008U)
#define RGX_MH_TAG_SB_TA_ENCODING_TA_TAG_VCERTC                                     (0x00000007U)
#define RGX_MH_TAG_SB_TA_ENCODING_TA_TAG_TEACRTC                                    (0x00000006U)
#define RGX_MH_TAG_SB_TA_ENCODING_TA_TAG_PSGRTC                                     (0x00000005U)
#define RGX_MH_TAG_SB_TA_ENCODING_TA_TAG_PSGR                                       (0x00000004U)
#define RGX_MH_TAG_SB_TA_ENCODING_TA_TAG_PSGS                                       (0x00000003U)
#define RGX_MH_TAG_SB_TA_ENCODING_TA_TAG_TPC                                        (0x00000002U)
#define RGX_MH_TAG_SB_TA_ENCODING_TA_TAG_VCE                                        (0x00000001U)


#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_2_ENCODING_IPF_TAG_CREQ00                   (0x00000000U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_2_ENCODING_IPF_TAG_CREQ01                   (0x00000001U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_2_ENCODING_IPF_TAG_PREQ00                   (0x00000002U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_2_ENCODING_IPF_TAG_PREQ01                   (0x00000003U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_2_ENCODING_IPF_TAG_RREQ                     (0x00000004U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_2_ENCODING_IPF_TAG_DBSC                     (0x00000005U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_2_ENCODING_IPF_TAG_CPF                      (0x00000006U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_2_ENCODING_IPF_TAG_DELTA                    (0x00000007U)


#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_4_ENCODING_IPF_TAG_CREQ00                   (0x00000000U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_4_ENCODING_IPF_TAG_CREQ01                   (0x00000001U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_4_ENCODING_IPF_TAG_CREQ02                   (0x00000002U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_4_ENCODING_IPF_TAG_CREQ03                   (0x00000003U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_4_ENCODING_IPF_TAG_PREQ00                   (0x00000004U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_4_ENCODING_IPF_TAG_PREQ01                   (0x00000005U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_4_ENCODING_IPF_TAG_PREQ02                   (0x00000006U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_4_ENCODING_IPF_TAG_PREQ03                   (0x00000007U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_4_ENCODING_IPF_TAG_RREQ                     (0x00000008U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_4_ENCODING_IPF_TAG_DBSC                     (0x00000009U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_4_ENCODING_IPF_TAG_CPF                      (0x0000000aU)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_4_ENCODING_IPF_TAG_DELTA                    (0x0000000bU)


#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_7_ENCODING_IPF_TAG_CREQ00                   (0x00000000U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_7_ENCODING_IPF_TAG_CREQ01                   (0x00000001U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_7_ENCODING_IPF_TAG_CREQ02                   (0x00000002U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_7_ENCODING_IPF_TAG_CREQ03                   (0x00000003U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_7_ENCODING_IPF_TAG_CREQ04                   (0x00000004U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_7_ENCODING_IPF_TAG_CREQ05                   (0x00000005U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_7_ENCODING_IPF_TAG_CREQ06                   (0x00000006U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_7_ENCODING_IPF_TAG_PREQ00                   (0x00000007U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_7_ENCODING_IPF_TAG_PREQ01                   (0x00000008U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_7_ENCODING_IPF_TAG_PREQ02                   (0x00000009U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_7_ENCODING_IPF_TAG_PREQ03                   (0x0000000aU)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_7_ENCODING_IPF_TAG_PREQ04                   (0x0000000bU)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_7_ENCODING_IPF_TAG_PREQ05                   (0x0000000cU)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_7_ENCODING_IPF_TAG_PREQ06                   (0x0000000dU)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_7_ENCODING_IPF_TAG_RREQ                     (0x0000000eU)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_7_ENCODING_IPF_TAG_DBSC                     (0x0000000fU)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_7_ENCODING_IPF_TAG_CPF                      (0x00000010U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_7_ENCODING_IPF_TAG_DELTA                    (0x00000011U)


#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_CREQ00                  (0x00000000U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_CREQ01                  (0x00000001U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_CREQ02                  (0x00000002U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_CREQ03                  (0x00000003U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_CREQ04                  (0x00000004U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_CREQ05                  (0x00000005U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_CREQ06                  (0x00000006U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_CREQ07                  (0x00000007U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_CREQ08                  (0x00000008U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_CREQ09                  (0x00000009U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_CREQ10                  (0x0000000aU)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_CREQ11                  (0x0000000bU)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_CREQ12                  (0x0000000cU)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_CREQ13                  (0x0000000dU)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_PREQ00                  (0x0000000eU)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_PREQ01                  (0x0000000fU)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_PREQ02                  (0x00000010U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_PREQ03                  (0x00000011U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_PREQ04                  (0x00000012U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_PREQ05                  (0x00000013U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_PREQ06                  (0x00000014U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_PREQ07                  (0x00000015U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_PREQ08                  (0x00000016U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_PREQ09                  (0x00000017U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_PREQ10                  (0x00000018U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_PREQ11                  (0x00000019U)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_PREQ12                  (0x0000001aU)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_PREQ13                  (0x0000001bU)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_RREQ                    (0x0000001cU)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_DBSC                    (0x0000001dU)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_CPF                     (0x0000001eU)
#define RGX_MH_TAG_SB_IPF_IPF_NUM_PIPES_14_ENCODING_IPF_TAG_DELTA                   (0x0000001fU)


#define RGX_MH_TAG_SB_TPF_ENCODING_TPF_TAG_PDS_STATE                                (0x00000000U)
#define RGX_MH_TAG_SB_TPF_ENCODING_TPF_TAG_DEPTH_BIAS                               (0x00000001U)
#define RGX_MH_TAG_SB_TPF_ENCODING_TPF_TAG_FLOOR_OFFSET_DATA                        (0x00000002U)
#define RGX_MH_TAG_SB_TPF_ENCODING_TPF_TAG_DELTA_DATA                               (0x00000003U)


#define RGX_MH_TAG_SB_ISP_ENCODING_ISP_TAG_ZLS                                      (0x00000000U)
#define RGX_MH_TAG_SB_ISP_ENCODING_ISP_TAG_DS                                       (0x00000001U)


#define RGX_MH_TAG_SB_VDM_ENCODING_VDM_TAG_CONTROL                                  (0x00000000U)
#define RGX_MH_TAG_SB_VDM_ENCODING_VDM_TAG_STATE                                    (0x00000001U)
#define RGX_MH_TAG_SB_VDM_ENCODING_VDM_TAG_INDEX                                    (0x00000002U)
#define RGX_MH_TAG_SB_VDM_ENCODING_VDM_TAG_STACK                                    (0x00000004U)
#define RGX_MH_TAG_SB_VDM_ENCODING_VDM_TAG_CONTEXT                                  (0x00000008U)


#define RGX_MH_TAG_SB_CDM_ENCODING_CDM_TAG_CONTROL_STREAM                           (0x00000000U)
#define RGX_MH_TAG_SB_CDM_ENCODING_CDM_TAG_INDIRECT_DATA                            (0x00000001U)
#define RGX_MH_TAG_SB_CDM_ENCODING_CDM_TAG_EVENT_DATA                               (0x00000002U)
#define RGX_MH_TAG_SB_CDM_ENCODING_CDM_TAG_CONTEXT_STATE                            (0x00000003U)


#define RGX_MH_TAG_SB_MIPS_ENCODING_MIPS_TAG_OPCODE_FETCH                           (0x00000002U)
#define RGX_MH_TAG_SB_MIPS_ENCODING_MIPS_TAG_DATA_ACCESS                            (0x00000003U)


#define RGX_MH_TAG_SB_MMU_ENCODING_MMU_TAG_PT_REQUEST                               (0x00000000U)
#define RGX_MH_TAG_SB_MMU_ENCODING_MMU_TAG_PD_REQUEST                               (0x00000001U)
#define RGX_MH_TAG_SB_MMU_ENCODING_MMU_TAG_PC_REQUEST                               (0x00000002U)
#define RGX_MH_TAG_SB_MMU_ENCODING_MMU_TAG_PM_PT_REQUEST                            (0x00000003U)
#define RGX_MH_TAG_SB_MMU_ENCODING_MMU_TAG_PM_PD_REQUEST                            (0x00000004U)
#define RGX_MH_TAG_SB_MMU_ENCODING_MMU_TAG_PM_PC_REQUEST                            (0x00000005U)
#define RGX_MH_TAG_SB_MMU_ENCODING_MMU_TAG_PM_PD_WREQUEST                           (0x00000006U)
#define RGX_MH_TAG_SB_MMU_ENCODING_MMU_TAG_PM_PC_WREQUEST                           (0x00000007U)


#define RGX_MH_TAG_ENCODING_MH_TAG_MMU                                              (0x00000000U)
#define RGX_MH_TAG_ENCODING_MH_TAG_CPU_MMU                                          (0x00000001U)
#define RGX_MH_TAG_ENCODING_MH_TAG_CPU_IFU                                          (0x00000002U)
#define RGX_MH_TAG_ENCODING_MH_TAG_CPU_LSU                                          (0x00000003U)
#define RGX_MH_TAG_ENCODING_MH_TAG_MIPS                                             (0x00000004U)
#define RGX_MH_TAG_ENCODING_MH_TAG_CDM_STG0                                         (0x00000005U)
#define RGX_MH_TAG_ENCODING_MH_TAG_CDM_STG1                                         (0x00000006U)
#define RGX_MH_TAG_ENCODING_MH_TAG_CDM_STG2                                         (0x00000007U)
#define RGX_MH_TAG_ENCODING_MH_TAG_CDM_STG3                                         (0x00000008U)
#define RGX_MH_TAG_ENCODING_MH_TAG_VDM_STG0                                         (0x00000009U)
#define RGX_MH_TAG_ENCODING_MH_TAG_VDM_STG1                                         (0x0000000aU)
#define RGX_MH_TAG_ENCODING_MH_TAG_VDM_STG2                                         (0x0000000bU)
#define RGX_MH_TAG_ENCODING_MH_TAG_VDM_STG3                                         (0x0000000cU)
#define RGX_MH_TAG_ENCODING_MH_TAG_VDM_STG4                                         (0x0000000dU)
#define RGX_MH_TAG_ENCODING_MH_TAG_PDS_0                                            (0x0000000eU)
#define RGX_MH_TAG_ENCODING_MH_TAG_PDS_1                                            (0x0000000fU)
#define RGX_MH_TAG_ENCODING_MH_TAG_MCU_USCA                                         (0x00000010U)
#define RGX_MH_TAG_ENCODING_MH_TAG_MCU_USCB                                         (0x00000011U)
#define RGX_MH_TAG_ENCODING_MH_TAG_MCU_USCC                                         (0x00000012U)
#define RGX_MH_TAG_ENCODING_MH_TAG_MCU_USCD                                         (0x00000013U)
#define RGX_MH_TAG_ENCODING_MH_TAG_MCU_PDS_USCA                                     (0x00000014U)
#define RGX_MH_TAG_ENCODING_MH_TAG_MCU_PDS_USCB                                     (0x00000015U)
#define RGX_MH_TAG_ENCODING_MH_TAG_MCU_PDS_USCC                                     (0x00000016U)
#define RGX_MH_TAG_ENCODING_MH_TAG_MCU_PDS_USCD                                     (0x00000017U)
#define RGX_MH_TAG_ENCODING_MH_TAG_MCU_PDSRW                                        (0x00000018U)
#define RGX_MH_TAG_ENCODING_MH_TAG_TCU_0                                            (0x00000019U)
#define RGX_MH_TAG_ENCODING_MH_TAG_TCU_1                                            (0x0000001aU)
#define RGX_MH_TAG_ENCODING_MH_TAG_FBCDC_0                                          (0x0000001bU)
#define RGX_MH_TAG_ENCODING_MH_TAG_FBCDC_1                                          (0x0000001cU)
#define RGX_MH_TAG_ENCODING_MH_TAG_FBCDC_2                                          (0x0000001dU)
#define RGX_MH_TAG_ENCODING_MH_TAG_FBCDC_3                                          (0x0000001eU)
#define RGX_MH_TAG_ENCODING_MH_TAG_USC                                              (0x0000001fU)
#define RGX_MH_TAG_ENCODING_MH_TAG_ISP_ZLS                                          (0x00000020U)
#define RGX_MH_TAG_ENCODING_MH_TAG_ISP_DS                                           (0x00000021U)
#define RGX_MH_TAG_ENCODING_MH_TAG_TPF                                              (0x00000022U)
#define RGX_MH_TAG_ENCODING_MH_TAG_TPF_PBCDBIAS                                     (0x00000023U)
#define RGX_MH_TAG_ENCODING_MH_TAG_TPF_SPF                                          (0x00000024U)
#define RGX_MH_TAG_ENCODING_MH_TAG_IPF_CREQ                                         (0x00000025U)
#define RGX_MH_TAG_ENCODING_MH_TAG_IPF_OTHERS                                       (0x00000026U)
#define RGX_MH_TAG_ENCODING_MH_TAG_VDM_STG5                                         (0x00000027U)
#define RGX_MH_TAG_ENCODING_MH_TAG_TA_PPP                                           (0x00000028U)
#define RGX_MH_TAG_ENCODING_MH_TAG_TA_TPWRTC                                        (0x00000029U)
#define RGX_MH_TAG_ENCODING_MH_TAG_TA_TEACRTC                                       (0x0000002aU)
#define RGX_MH_TAG_ENCODING_MH_TAG_TA_PSGRTC                                        (0x0000002bU)
#define RGX_MH_TAG_ENCODING_MH_TAG_TA_PSGREGION                                     (0x0000002cU)
#define RGX_MH_TAG_ENCODING_MH_TAG_TA_PSGSTREAM                                     (0x0000002dU)
#define RGX_MH_TAG_ENCODING_MH_TAG_TA_TPW                                           (0x0000002eU)
#define RGX_MH_TAG_ENCODING_MH_TAG_TA_TPC                                           (0x0000002fU)
#define RGX_MH_TAG_ENCODING_MH_TAG_PM_ALLOC                                         (0x00000030U)
#define RGX_MH_TAG_ENCODING_MH_TAG_PM_DEALLOC                                       (0x00000031U)
#define RGX_MH_TAG_ENCODING_MH_TAG_TDM_DMA                                          (0x00000032U)
#define RGX_MH_TAG_ENCODING_MH_TAG_TDM_CTL                                          (0x00000033U)
#define RGX_MH_TAG_ENCODING_MH_TAG_PBE0                                             (0x00000034U)
#define RGX_MH_TAG_ENCODING_MH_TAG_PBE1                                             (0x00000035U)
#define RGX_MH_TAG_ENCODING_MH_TAG_PBE2                                             (0x00000036U)
#define RGX_MH_TAG_ENCODING_MH_TAG_PBE3                                             (0x00000037U)


#endif /* RGXMHDEFS_KM_H */
/*****************************************************************************
 End of file (rgxmhdefs_km.h)
*****************************************************************************/
