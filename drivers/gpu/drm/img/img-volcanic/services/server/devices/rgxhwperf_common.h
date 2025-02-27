/*************************************************************************/ /*!
@File
@Title          RGX HW Performance header file
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
@Description    Header for the RGX HWPerf functions
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

#ifndef RGXHWPERF_COMMON_H_
#define RGXHWPERF_COMMON_H_

#include "img_types.h"
#include "img_defs.h"
#include "pvrsrv_error.h"

#include "device.h"
#include "connection_server.h"
#include "rgxdevice.h"
#include "rgx_hwperf.h"
#include "rgx_fwif_hwperf.h"
#if defined(PVR_ARCH_VOLCANIC)
#include "rgx_hwperf_table.h"
#endif
#include "cache_ops.h"
#include "rgxfwmemctx.h"

/* HWPerf host buffer size constraints in KBs */
#define HWPERF_HOST_TL_STREAM_SIZE_DEFAULT PVRSRV_APPHINT_HWPERFHOSTBUFSIZEINKB
#define HWPERF_HOST_TL_STREAM_SIZE_MIN     (32U)
#define HWPERF_HOST_TL_STREAM_SIZE_MAX     (3072U)

/* Operations on HWPerf filter. */
typedef enum HWPERF_FILTER_OPERATION_TAG
{
	HWPERF_FILTER_OPERATION_SET,
	HWPERF_FILTER_OPERATION_BIT_CLR,
	HWPERF_FILTER_OPERATION_BIT_OR,
} HWPERF_FILTER_OPERATION;

/******************************************************************************
 * RGX HW Performance decode Bvnc Features for HWPerf
 *****************************************************************************/
PVRSRV_ERROR RGXServerFeatureFlagsToHWPerfFlags(PVRSRV_RGXDEV_INFO *psDevInfo,
		                                        RGX_HWPERF_BVNC    *psBVNC);

PVRSRV_ERROR PVRSRVRGXGetHWPerfBvncFeatureFlagsKM(CONNECTION_DATA    *psConnection,
                                                  PVRSRV_DEVICE_NODE *psDeviceNode,
											      RGX_HWPERF_BVNC    *psBVNC);

/******************************************************************************
 * RGX HW Performance Data Transport Routines
 *****************************************************************************/

PVRSRV_ERROR RGXHWPerfDataStoreCB(PVRSRV_DEVICE_NODE* psDevInfo);

PVRSRV_ERROR RGXHWPerfInit(PVRSRV_RGXDEV_INFO *psRgxDevInfo);
void RGXHWPerfDeinit(PVRSRV_RGXDEV_INFO *psRgxDevInfo);

PVRSRV_ERROR RGXHWPerfInitOnDemandL1Buffer(PVRSRV_RGXDEV_INFO* psRgxDevInfo);
PVRSRV_ERROR RGXHWPerfInitOnDemandL2Stream(PVRSRV_RGXDEV_INFO* psRgxDevInfo,
                                           RGX_HWPERF_L2_STREAM_ID eL2StreamId);
void RGXHWPerfDeinitL2Stream(PVRSRV_RGXDEV_INFO *psRgxDevInfo,
                             RGX_HWPERF_L2_STREAM_ID eL2StreamId);
void RGXHWPerfInitAppHintCallbacks(const PVRSRV_DEVICE_NODE *psDeviceNode);
void RGXHWPerfClientInitAppHintCallbacks(void);

static INLINE PVRSRV_ERROR RGXAcquireHWPerfCtlCPUAddr(PVRSRV_DEVICE_NODE *psDevNode,
                                                      RGXFWIF_HWPERF_CTL **ppsHWPerfCtl)
{
	PVRSRV_RGXDEV_INFO *psDevInfo;
	PVRSRV_ERROR eError;

	PVR_RETURN_IF_FALSE(psDevNode != NULL, PVRSRV_ERROR_INVALID_PARAMS);

	psDevInfo = (PVRSRV_RGXDEV_INFO*)psDevNode->pvDevice;
	PVR_RETURN_IF_FALSE(psDevInfo != NULL, PVRSRV_ERROR_INVALID_PARAMS);

	eError = DevmemAcquireCpuVirtAddr(psDevInfo->psRGXFWIfHWPerfCountersMemDesc,
	                                  (void**)ppsHWPerfCtl);
	RGXFwSharedMemCacheOpPtr(ppsHWPerfCtl, INVALIDATE);

	return eError;
}

static INLINE void RGXReleaseHWPerfCtlCPUAddr(PVRSRV_DEVICE_NODE *psDevNode)
{
	PVRSRV_RGXDEV_INFO *psDevInfo;

	PVR_LOG_RETURN_VOID_IF_FALSE(psDevNode != NULL, "psDevNode is invalid");

	psDevInfo = (PVRSRV_RGXDEV_INFO*)psDevNode->pvDevice;
	PVR_LOG_RETURN_VOID_IF_FALSE(psDevInfo != NULL, "psDevInfo invalid");

	DevmemReleaseCpuVirtAddr(psDevInfo->psRGXFWIfHWPerfCountersMemDesc);
}

PVRSRV_ERROR PVRSRVRGXGetConfiguredHWPerfCountersKM(CONNECTION_DATA *psConnection,
                                                    PVRSRV_DEVICE_NODE *psDeviceNode,
                                                    const IMG_UINT32 ui32BlockID,
                                                    RGX_HWPERF_CONFIG_CNTBLK *psConfiguredCounters);

PVRSRV_ERROR PVRSRVRGXGetEnabledHWPerfBlocksKM(CONNECTION_DATA *psConnection,
                                               PVRSRV_DEVICE_NODE *psDeviceNode,
                                               const IMG_UINT32 ui32ArrayLen,
                                               IMG_UINT32 *pui32BlockCount,
                                               IMG_UINT32 *pui32EnabledBlockIDs);

/******************************************************************************
 * RGX HW Performance Profiling API(s)
 *****************************************************************************/

PVRSRV_ERROR PVRSRVRGXCtrlHWPerfKM(
	CONNECTION_DATA      * psConnection,
	PVRSRV_DEVICE_NODE   * psDeviceNode,
	 RGX_HWPERF_STREAM_ID  eStreamId,
	IMG_BOOL               bToggle,
	IMG_UINT64             ui64Mask);

PVRSRV_ERROR PVRSRVRGXCtrlHWPerfFW(
	PVRSRV_DEVICE_NODE      *psDeviceNode,
	RGX_HWPERF_L2_STREAM_ID  eL2StreamId,
	IMG_UINT64               ui64Mask,
	HWPERF_FILTER_OPERATION  eMaskOp);

PVRSRV_ERROR PVRSRVRGXGetHWPerfTimeStampKM(
		CONNECTION_DATA         *psConnection,
		PVRSRV_DEVICE_NODE      *psDeviceNode,
		IMG_UINT64              *pui64TimeStamp);

PVRSRV_ERROR PVRSRVRGXControlHWPerfBlocksKM(
	CONNECTION_DATA       * psConnection,
	PVRSRV_DEVICE_NODE    * psDeviceNode,
	IMG_BOOL              bEnable,
	IMG_UINT32            ui32ArrayLen,
	IMG_UINT16            * psBlockIDs);

/******************************************************************************
 * RGX HW Performance Host Stream API
 *****************************************************************************/

static inline RGX_HWPERF_HOST_DEVICE_HEALTH_STATUS
RGXHWPerfConvDeviceHealthStatus(PVRSRV_DEVICE_HEALTH_STATUS eDeviceHealthStatus)
{
	switch (eDeviceHealthStatus)
	{
		case PVRSRV_DEVICE_HEALTH_STATUS_UNDEFINED:         return RGX_HWPERF_HOST_DEVICE_HEALTH_STATUS_UNDEFINED;
		case PVRSRV_DEVICE_HEALTH_STATUS_OK:                return RGX_HWPERF_HOST_DEVICE_HEALTH_STATUS_OK;
		case PVRSRV_DEVICE_HEALTH_STATUS_NOT_RESPONDING:    return RGX_HWPERF_HOST_DEVICE_HEALTH_STATUS_NOT_RESPONDING;
		case PVRSRV_DEVICE_HEALTH_STATUS_DEAD:              return RGX_HWPERF_HOST_DEVICE_HEALTH_STATUS_DEAD;
		case PVRSRV_DEVICE_HEALTH_STATUS_FAULT:             return RGX_HWPERF_HOST_DEVICE_HEALTH_STATUS_FAULT;
		default:                                            return RGX_HWPERF_HOST_DEVICE_HEALTH_STATUS_UNDEFINED;
	}
}

static inline RGX_HWPERF_HOST_DEVICE_HEALTH_REASON
RGXHWPerfConvDeviceHealthReason(PVRSRV_DEVICE_HEALTH_REASON eDeviceHealthReason)
{
	switch (eDeviceHealthReason)
	{
		case PVRSRV_DEVICE_HEALTH_REASON_NONE:                  return RGX_HWPERF_HOST_DEVICE_HEALTH_REASON_NONE;
		case PVRSRV_DEVICE_HEALTH_REASON_ASSERTED:              return RGX_HWPERF_HOST_DEVICE_HEALTH_REASON_ASSERTED;
		case PVRSRV_DEVICE_HEALTH_REASON_POLL_FAILING:          return RGX_HWPERF_HOST_DEVICE_HEALTH_REASON_POLL_FAILING;
		case PVRSRV_DEVICE_HEALTH_REASON_TIMEOUTS:              return RGX_HWPERF_HOST_DEVICE_HEALTH_REASON_TIMEOUTS;
		case PVRSRV_DEVICE_HEALTH_REASON_QUEUE_CORRUPT:         return RGX_HWPERF_HOST_DEVICE_HEALTH_REASON_QUEUE_CORRUPT;
		case PVRSRV_DEVICE_HEALTH_REASON_QUEUE_STALLED:         return RGX_HWPERF_HOST_DEVICE_HEALTH_REASON_QUEUE_STALLED;
		case PVRSRV_DEVICE_HEALTH_REASON_IDLING:                return RGX_HWPERF_HOST_DEVICE_HEALTH_REASON_IDLING;
		case PVRSRV_DEVICE_HEALTH_REASON_RESTARTING:            return RGX_HWPERF_HOST_DEVICE_HEALTH_REASON_RESTARTING;
		case PVRSRV_DEVICE_HEALTH_REASON_MISSING_INTERRUPTS:    return RGX_HWPERF_HOST_DEVICE_HEALTH_REASON_MISSING_INTERRUPTS;
		case PVRSRV_DEVICE_HEALTH_REASON_PCI_ERROR:             return RGX_HWPERF_HOST_DEVICE_HEALTH_REASON_PCI_ERROR;
		default:                                                return RGX_HWPERF_HOST_DEVICE_HEALTH_REASON_UNDEFINED;
	}
}

PVRSRV_ERROR RGXHWPerfHostInit(PVRSRV_RGXDEV_INFO *psRgxDevInfo, IMG_UINT32 ui32BufSizeKB);
PVRSRV_ERROR RGXHWPerfHostInitOnDemandResources(PVRSRV_RGXDEV_INFO* psRgxDevInfo);
void RGXHWPerfHostDeInit(PVRSRV_RGXDEV_INFO	*psRgxDevInfo);

IMG_UINT64 RGXHWPerfFwSetEventFilter(PVRSRV_RGXDEV_INFO *psRgxDevInfo,
                                     RGX_HWPERF_L2_STREAM_ID eL2StreamId,
                                     IMG_UINT64 uiFilter);

void RGXHWPerfHostSetEventFilter(PVRSRV_RGXDEV_INFO *psRgxDevInfo,
                                 IMG_UINT32 ui32Filter);

void RGXHWPerfHostPostRaw(PVRSRV_RGXDEV_INFO *psRgxDevInfo,
						  RGX_HWPERF_HOST_EVENT_TYPE eEvType,
						  IMG_BYTE *pbPayload,
						  IMG_UINT32 ui32PayloadSize);

void RGXHWPerfHostPostEnqEvent(PVRSRV_RGXDEV_INFO *psRgxDevInfo,
                               RGX_HWPERF_KICK_TYPE eEnqType,
                               IMG_UINT32 ui32Pid,
                               IMG_UINT32 ui32FWDMContext,
                               IMG_UINT32 ui32ExtJobRef,
                               IMG_UINT32 ui32IntJobRef,
                               PVRSRV_FENCE hCheckFence,
                               PVRSRV_FENCE hUpdateFence,
                               PVRSRV_TIMELINE hUpdateTimeline,
                               IMG_UINT64 ui64CheckFenceUID,
                               IMG_UINT64 ui64UpdateFenceUID,
                               IMG_UINT64 ui64DeadlineInus,
                               IMG_UINT32 ui32CycleEstimate);

void RGXHWPerfHostPostAllocEvent(PVRSRV_RGXDEV_INFO *psRgxDevInfo,
                                 RGX_HWPERF_HOST_RESOURCE_TYPE eAllocType,
                                 const IMG_CHAR *psName,
                                 IMG_UINT32 ui32NameSize,
                                 RGX_HWPERF_HOST_ALLOC_DETAIL *puAllocDetail);

void RGXHWPerfHostPostFreeEvent(PVRSRV_RGXDEV_INFO *psRgxDevInfo,
                                RGX_HWPERF_HOST_RESOURCE_TYPE eFreeType,
                                IMG_UINT64 ui64UID,
                                IMG_UINT32 ui32PID,
                                IMG_UINT32 ui32FWAddr);

void RGXHWPerfHostPostModifyEvent(PVRSRV_RGXDEV_INFO *psRgxDevInfo,
                                  RGX_HWPERF_HOST_RESOURCE_TYPE eModifyType,
                                  IMG_UINT64 ui64NewUID,
                                  IMG_UINT64 ui64UID1,
                                  IMG_UINT64 ui64UID2,
                                  const IMG_CHAR *psName,
                                  IMG_UINT32 ui32NameSize);

void RGXHWPerfHostPostUfoEvent(PVRSRV_RGXDEV_INFO *psRgxDevInfo,
                               RGX_HWPERF_UFO_EV eUfoType,
                               RGX_HWPERF_UFO_DATA_ELEMENT *psUFOData,
							   const IMG_BOOL bSleepAllowed);

void RGXHWPerfHostPostClkSyncEvent(PVRSRV_RGXDEV_INFO *psRgxDevInfo);

void RGXHWPerfHostPostDeviceInfo(PVRSRV_RGXDEV_INFO *psRgxDevInfo,
								 RGX_HWPERF_DEV_INFO_EV eEvType,
								 RGX_HWPERF_HOST_DEV_INFO_DETAIL *psData);

void RGXHWPerfHostPostInfo(PVRSRV_RGXDEV_INFO *psRgxDevInfo,
								 RGX_HWPERF_INFO_EV eEvType);

void RGXHWPerfHostPostFenceWait(PVRSRV_RGXDEV_INFO *psRgxDevInfo,
								RGX_HWPERF_HOST_SYNC_FENCE_WAIT_TYPE eType,
								IMG_PID uiPID,
								PVRSRV_FENCE hFence,
								IMG_UINT32 ui32Data);

void RGXHWPerfHostPostSWTimelineAdv(PVRSRV_RGXDEV_INFO *psRgxDevInfo,
                                    IMG_PID uiPID,
									PVRSRV_TIMELINE hSWTimeline,
									IMG_UINT64 ui64SyncPtIndex);

void RGXHWPerfHostPostClientInfoProcName(PVRSRV_RGXDEV_INFO *psRgxDevInfo,
                                         IMG_PID uiPID,
									     const IMG_CHAR *psName);

IMG_BOOL RGXHWPerfHostIsEventEnabled(PVRSRV_RGXDEV_INFO *psRgxDevInfo, RGX_HWPERF_HOST_EVENT_TYPE eEvent);

#if defined(PVR_ARCH_VOLCANIC)
IMG_INTERNAL /*static inline*/ IMG_UINT32
RGXGetHWPerfBlockConfig(const RGXFW_HWPERF_CNTBLK_TYPE_MODEL **ppsModel);

/*!
*******************************************************************************
 @Function    RGXHWPerfMaxDefinedBlks

 @Description Return the number of valid block-IDs for the given device node

 @Input       (PVRSRV_RGXDEV_INFO *)   pvDevice    device-node to query

 @Returns     (IMG_UINT32)             Number of block-IDs (RGX_CNTBLK_ID)
                                       valid for this device.
******************************************************************************/
IMG_INTERNAL IMG_UINT32
RGXHWPerfMaxDefinedBlks(PVRSRV_RGXDEV_INFO *psDevInfo);
#endif

#define _RGX_HWPERF_HOST_FILTER(CTX, EV) \
		(((PVRSRV_RGXDEV_INFO *)CTX->psDeviceNode->pvDevice)->ui32HWPerfHostFilter \
		& RGX_HWPERF_EVENT_MASK_VALUE(EV))

#define _RGX_DEVICE_INFO_FROM_CTX(CTX) \
		((PVRSRV_RGXDEV_INFO *)CTX->psDeviceNode->pvDevice)

#define _RGX_DEVICE_INFO_FROM_NODE(DEVNODE) \
		((PVRSRV_RGXDEV_INFO *)DEVNODE->pvDevice)

/* Deadline and cycle estimate is not supported for all ENQ events */
#define NO_DEADLINE 0
#define NO_CYCEST   0


#if defined(SUPPORT_RGX)

/**
 * This macro checks if HWPerfHost and the event are enabled and if they are
 * it posts event to the HWPerfHost stream.
 *
 * @param C      Kick context
 * @param P      Pid of kicking process
 * @param X      Related FW context
 * @param E      External job reference
 * @param I      Job ID
 * @param K      Kick type
 * @param CF     Check fence handle
 * @param UF     Update fence handle
 * @param UT     Update timeline (on which above UF was created) handle
 * @param CHKUID Check fence UID
 * @param UPDUID Update fence UID
 * @param D      Deadline
 * @param CE     Cycle estimate
 */
#define RGXSRV_HWPERF_ENQ(C, P, X, E, I, K, CF, UF, UT, CHKUID, UPDUID, D, CE) \
		do { \
			if (_RGX_HWPERF_HOST_FILTER(C, RGX_HWPERF_HOST_ENQ)) \
			{ \
				RGXHWPerfHostPostEnqEvent(_RGX_DEVICE_INFO_FROM_CTX(C), \
				                          (K), (P), (X), (E), (I), \
				                          (CF), (UF), (UT), \
				                          (CHKUID), (UPDUID), (D), (CE)); \
			} \
		} while (0)

/**
 * This macro checks if HWPerfHost and the event are enabled and if they are
 * it posts event to the HWPerfHost stream.
 *
 * @param I Device Info pointer
 * @param T Host UFO event type
 * @param D Pointer to UFO data
 * @param S Is sleeping allowed?
 */
#define RGXSRV_HWPERF_UFO(I, T, D, S) \
		do { \
			if (RGXHWPerfHostIsEventEnabled((I), RGX_HWPERF_HOST_UFO)) \
			{ \
				RGXHWPerfHostPostUfoEvent((I), (T), (D), (S)); \
			} \
		} while (0)

/**
 * This macro checks if HWPerfHost and the event are enabled and if they are
 * it posts event to the HWPerfHost stream.
 *
 * @param D Device node pointer
 * @param T Host ALLOC event type
 * @param FWADDR sync firmware address
 * @param N string containing sync name
 * @param Z string size including null terminating character
 */
#define RGXSRV_HWPERF_ALLOC(D, T, FWADDR, N, Z) \
		do { \
			if (RGXHWPerfHostIsEventEnabled(_RGX_DEVICE_INFO_FROM_NODE(D), RGX_HWPERF_HOST_ALLOC)) \
			{ \
				RGX_HWPERF_HOST_ALLOC_DETAIL uAllocDetail; \
				uAllocDetail.sSyncAlloc.ui32FWAddr = (FWADDR); \
				RGXHWPerfHostPostAllocEvent(_RGX_DEVICE_INFO_FROM_NODE(D), \
				                            RGX_HWPERF_HOST_RESOURCE_TYPE_##T, \
											(N), (Z), &uAllocDetail); \
			} \
		} while (0)

/**
 * This macro checks if HWPerfHost and the event are enabled and if they are
 * it posts event to the HWPerfHost stream.
 *
 * @param D Device Node pointer
 * @param PID ID of allocating process
 * @param FENCE PVRSRV_FENCE object
 * @param FWADDR sync firmware address
 * @param N string containing sync name
 * @param Z string size including null terminating character
 */
#define RGXSRV_HWPERF_ALLOC_FENCE(D, PID, FENCE, FWADDR, N, Z)  \
		do { \
			if (RGXHWPerfHostIsEventEnabled(_RGX_DEVICE_INFO_FROM_NODE(D), RGX_HWPERF_HOST_ALLOC)) \
			{ \
				RGX_HWPERF_HOST_ALLOC_DETAIL uAllocDetail; \
				uAllocDetail.sFenceAlloc.uiPID = (PID); \
				uAllocDetail.sFenceAlloc.hFence = (FENCE); \
				uAllocDetail.sFenceAlloc.ui32CheckPt_FWAddr = (FWADDR); \
				RGXHWPerfHostPostAllocEvent(_RGX_DEVICE_INFO_FROM_NODE(D), \
				                            RGX_HWPERF_HOST_RESOURCE_TYPE_FENCE_PVR, \
				                            N, Z, &uAllocDetail); \
			} \
		} while (0)

/**
 * @param D Device Node pointer
 * @param TL PVRSRV_TIMELINE on which CP is allocated
 * @param PID Allocating process ID of this TL/FENCE
 * @param FENCE PVRSRV_FENCE as passed to SyncCheckpointResolveFence OR PVRSRV_NO_FENCE
 * @param FWADDR sync firmware address
 * @param N string containing sync name
 * @param Z string size including null terminating character
 */
#define RGXSRV_HWPERF_ALLOC_SYNC_CP(D, TL, PID, FENCE, FWADDR, N, Z)  \
		do { \
			if (RGXHWPerfHostIsEventEnabled(_RGX_DEVICE_INFO_FROM_NODE(D), RGX_HWPERF_HOST_ALLOC)) \
			{ \
				RGX_HWPERF_HOST_ALLOC_DETAIL uAllocDetail; \
				uAllocDetail.sSyncCheckPointAlloc.ui32CheckPt_FWAddr = (FWADDR); \
				uAllocDetail.sSyncCheckPointAlloc.hTimeline = (TL); \
				uAllocDetail.sSyncCheckPointAlloc.uiPID = (PID); \
				uAllocDetail.sSyncCheckPointAlloc.hFence = (FENCE); \
				RGXHWPerfHostPostAllocEvent(_RGX_DEVICE_INFO_FROM_NODE(D), \
				                            RGX_HWPERF_HOST_RESOURCE_TYPE_SYNC_CP, \
				                            N, Z, &uAllocDetail); \
			} \
		} while (0)

/**
 * @param D Device Node pointer
 * @param PID ID of allocating process
 * @param SW_FENCE PVRSRV_FENCE object
 * @param SW_TL PVRSRV_TIMELINE on which SW_FENCE is allocated
 * @param SPI Sync point index on the SW_TL on which this SW_FENCE is allocated
 * @param N string containing sync name
 * @param Z string size including null terminating character
 */
#define RGXSRV_HWPERF_ALLOC_SW_FENCE(D, PID, SW_FENCE, SW_TL, SPI, N, Z)  \
		do { \
			if (RGXHWPerfHostIsEventEnabled(_RGX_DEVICE_INFO_FROM_NODE(D), RGX_HWPERF_HOST_ALLOC)) \
			{ \
				RGX_HWPERF_HOST_ALLOC_DETAIL uAllocDetail; \
				uAllocDetail.sSWFenceAlloc.uiPID = (PID); \
				uAllocDetail.sSWFenceAlloc.hSWFence = (SW_FENCE); \
				uAllocDetail.sSWFenceAlloc.hSWTimeline = (SW_TL); \
				uAllocDetail.sSWFenceAlloc.ui64SyncPtIndex = (SPI); \
				RGXHWPerfHostPostAllocEvent(_RGX_DEVICE_INFO_FROM_NODE(D), \
				                            RGX_HWPERF_HOST_RESOURCE_TYPE_FENCE_SW, \
				                            N, Z, &uAllocDetail); \
			} \
		} while (0)

/**
 * This macro checks if HWPerfHost and the event are enabled and if they are
 * it posts event to the HWPerfHost stream.
 *
 * @param D Device Node pointer
 * @param T Host ALLOC event type
 * @param FWADDR sync firmware address
 */
#define RGXSRV_HWPERF_FREE(D, T, FWADDR) \
		do { \
			if (RGXHWPerfHostIsEventEnabled(_RGX_DEVICE_INFO_FROM_NODE(D), RGX_HWPERF_HOST_FREE)) \
			{ \
				RGXHWPerfHostPostFreeEvent(_RGX_DEVICE_INFO_FROM_NODE(D), \
				                           RGX_HWPERF_HOST_RESOURCE_TYPE_##T, \
				                           (0), (0), (FWADDR)); \
			} \
		} while (0)

/**
 * This macro checks if HWPerfHost and the event are enabled and if they are
 * it posts event to the HWPerfHost stream.
 *
 * @param D Device Node pointer
 * @param T Host ALLOC event type
 * @param UID ID of input object
 * @param PID ID of allocating process
 * @param FWADDR sync firmware address
 */
#define RGXSRV_HWPERF_FREE_FENCE_SYNC(D, T, UID, PID, FWADDR) \
		do { \
			if (RGXHWPerfHostIsEventEnabled(_RGX_DEVICE_INFO_FROM_NODE(D), RGX_HWPERF_HOST_FREE)) \
			{ \
				RGXHWPerfHostPostFreeEvent(_RGX_DEVICE_INFO_FROM_NODE(D), \
				                           RGX_HWPERF_HOST_RESOURCE_TYPE_##T, \
				                           (UID), (PID), (FWADDR)); \
			} \
		} while (0)

/**
 * This macro checks if HWPerfHost and the event are enabled and if they are
 * it posts event to the HWPerfHost stream.
 *
 * @param D Device Node pointer
 * @param T Host ALLOC event type
 * @param NEWUID ID of output object
 * @param UID1 ID of first input object
 * @param UID2 ID of second input object
 * @param N string containing new object's name
 * @param Z string size including null terminating character
 */
#define RGXSRV_HWPERF_MODIFY_FENCE_SYNC(D, T, NEWUID, UID1, UID2, N, Z) \
		do { \
			if (RGXHWPerfHostIsEventEnabled(_RGX_DEVICE_INFO_FROM_NODE(D), RGX_HWPERF_HOST_MODIFY)) \
			{ \
				RGXHWPerfHostPostModifyEvent(_RGX_DEVICE_INFO_FROM_NODE(D), \
				                             RGX_HWPERF_HOST_RESOURCE_TYPE_##T, \
				                             (NEWUID), (UID1), (UID2), N, Z); \
			} \
		} while (0)


/**
 * This macro checks if HWPerfHost and the event are enabled and if they are
 * it posts event to the HWPerfHost stream.
 *
 * @param I Device info pointer
 */
#define RGXSRV_HWPERF_CLK_SYNC(I) \
		do { \
			if (RGXHWPerfHostIsEventEnabled((I), RGX_HWPERF_HOST_CLK_SYNC)) \
			{ \
				RGXHWPerfHostPostClkSyncEvent((I)); \
			} \
		} while (0)


/**
 * This macro checks if HWPerfHost and the event are enabled and if they are
 * it posts a device info health event to the HWPerfHost stream.
 *
 * @param I      Device info pointer
 * @param H      Health status enum
 * @param R      Health reason enum
 */
#define RGXSRV_HWPERF_DEVICE_INFO_HEALTH(I, H, R) \
	do { \
		if (RGXHWPerfHostIsEventEnabled((I), RGX_HWPERF_HOST_DEV_INFO)) \
		{ \
			RGX_HWPERF_HOST_DEV_INFO_DETAIL uDevDetail; \
			uDevDetail.sDeviceStatus.eDeviceHealthStatus = RGXHWPerfConvDeviceHealthStatus(H); \
			uDevDetail.sDeviceStatus.eDeviceHealthReason = RGXHWPerfConvDeviceHealthReason(R); \
			RGXHWPerfHostPostDeviceInfo((I), RGX_HWPERF_DEV_INFO_EV_HEALTH, &uDevDetail); \
		} \
	} while (0)

/**
 * This macro checks if HWPerfHost and the event are enabled and if they are
 * it posts a device info features event to the HWPerfHost stream.
 *
 * @param I      Device info pointer
 */
#define RGXSRV_HWPERF_DEVICE_INFO_FEATURES(I) \
	do { \
		if (RGXHWPerfHostIsEventEnabled((I), RGX_HWPERF_HOST_DEV_INFO)) \
		{ \
			RGXHWPerfHostPostDeviceInfo((I), RGX_HWPERF_DEV_INFO_EV_FEATURES, NULL); \
		} \
	} while (0)

/**
 * This macro checks if HWPerfHost and the event are enabled and if they are
 * it posts event to the HWPerfHost stream.
 *
 * @param I      Device info pointer
 * @param T      Event type
 */
#define RGXSRV_HWPERF_HOST_INFO(I, T) \
do { \
	if (RGXHWPerfHostIsEventEnabled((I), RGX_HWPERF_HOST_INFO)) \
	{ \
		RGXHWPerfHostPostInfo((I), (T)); \
	} \
} while (0)

/**
 * @param I      Device info pointer
 * @param T      Wait Event type
 * @param PID    Process ID that the following fence belongs to
 * @param F      Fence handle
 * @param D      Data for this wait event type
 */
#define RGXSRV_HWPERF_SYNC_FENCE_WAIT(I, T, PID, F, D) \
do { \
	if (RGXHWPerfHostIsEventEnabled((I), RGX_HWPERF_HOST_SYNC_FENCE_WAIT)) \
	{ \
		RGXHWPerfHostPostFenceWait(I, RGX_HWPERF_HOST_SYNC_FENCE_WAIT_TYPE_##T, \
		                           (PID), (F), (D)); \
	} \
} while (0)

/**
 * @param I      Device info pointer
 * @param PID    Process ID that the following timeline belongs to
 * @param F      SW-timeline handle
 * @param SPI    Sync-pt index where this SW-timeline has reached
 */
#define RGXSRV_HWPERF_SYNC_SW_TL_ADV(I, PID, SW_TL, SPI)\
do { \
	if (RGXHWPerfHostIsEventEnabled((I), RGX_HWPERF_HOST_SYNC_SW_TL_ADVANCE)) \
	{ \
		RGXHWPerfHostPostSWTimelineAdv((I), (PID), (SW_TL), (SPI)); \
	} \
} while (0)

/**
 * @param D      Device Node pointer
 * @param PID    Process ID that the following timeline belongs to
 * @param N      Null terminated string containing the process name
 */
#define RGXSRV_HWPERF_HOST_CLIENT_INFO_PROCESS_NAME(D, PID, N) \
do { \
	if (RGXHWPerfHostIsEventEnabled(_RGX_DEVICE_INFO_FROM_NODE(D), RGX_HWPERF_HOST_CLIENT_INFO)) \
	{ \
		RGXHWPerfHostPostClientInfoProcName(_RGX_DEVICE_INFO_FROM_NODE(D), (PID), (N)); \
	} \
} while (0)

#else

#define RGXSRV_HWPERF_ENQ(C, P, X, E, I, K, CF, UF, UT, CHKUID, UPDUID, D, CE)
#define RGXSRV_HWPERF_UFO(I, T, D, S)
#define RGXSRV_HWPERF_ALLOC(D, T, FWADDR, N, Z)
#define RGXSRV_HWPERF_ALLOC_FENCE(D, PID, FENCE, FWADDR, N, Z)
#define RGXSRV_HWPERF_ALLOC_SYNC_CP(D, TL, PID, FENCE, FWADDR, N, Z)
#define RGXSRV_HWPERF_ALLOC_SW_FENCE(D, PID, SW_FENCE, SW_TL, SPI, N, Z)
#define RGXSRV_HWPERF_FREE(D, T, FWADDR)
#define RGXSRV_HWPERF_FREE_FENCE_SYNC(D, T, UID, PID, FWADDR)
#define RGXSRV_HWPERF_MODIFY_FENCE_SYNC(D, T, NEWUID, UID1, UID2, N, Z)
#define RGXSRV_HWPERF_CLK_SYNC(I)
#define RGXSRV_HWPERF_DEVICE_INFO_HEALTH(I, H, R)
#define RGXSRV_HWPERF_DEVICE_INFO_FEATURES(I)
#define RGXSRV_HWPERF_HOST_INFO(I, T)
#define RGXSRV_HWPERF_SYNC_FENCE_WAIT(I, T, PID, F, D)
#define RGXSRV_HWPERF_SYNC_SW_TL_ADV(I, PID, SW_TL, SPI)
#define RGXSRV_HWPERF_HOST_CLIENT_INFO_PROCESS_NAME(D, PID, N)

#endif

#endif /* RGXHWPERF_COMMON_H_ */
