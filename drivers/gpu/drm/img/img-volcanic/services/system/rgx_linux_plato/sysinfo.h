/*************************************************************************/ /*!
@File
@Title          System Description Header
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
@Description    This header provides system-specific declarations and macros
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

#if !defined(SYSINFO_H)
#define SYSINFO_H

#if defined(__KERNEL__)
#include "plato_drv.h"
#endif

#define SYS_RGX_DEV_VENDOR_ID	(0x1AEE)
#define SYS_RGX_DEV_DEVICE_ID	(0x0003)

#if defined(__KERNEL__)
#if defined(PLATO_MULTI_DEVICE)
#define SYS_RGX_DEV_NAME_0	PLATO_MAKE_DEVICE_NAME_ROGUE(0)
#define SYS_RGX_DEV_NAME_1	PLATO_MAKE_DEVICE_NAME_ROGUE(1)
#define SYS_RGX_DEV_NAME_2	PLATO_MAKE_DEVICE_NAME_ROGUE(2)
#define SYS_RGX_DEV_NAME_3	PLATO_MAKE_DEVICE_NAME_ROGUE(3)
#else
#define SYS_RGX_DEV_NAME	PLATO_DEVICE_NAME_ROGUE
#endif
#endif

/*!< System specific poll/timeout details */
#if defined(VIRTUAL_PLATFORM) || defined(EMULATOR)
/* Emulator clock ~600 times slower than HW */
#define MAX_HW_TIME_US                           (300000000)
#define DEVICES_WATCHDOG_POWER_ON_SLEEP_TIMEOUT  (1000000)

#if defined(VIRTUAL_PLATFORM)
#define EVENT_OBJECT_TIMEOUT_US                  (120000000)
#elif defined(EMULATOR)
#define EVENT_OBJECT_TIMEOUT_US                  (2000000)
#endif

#else
#define MAX_HW_TIME_US                           (500000)
#define DEVICES_WATCHDOG_POWER_ON_SLEEP_TIMEOUT  (1500)//(100000)
#define EVENT_OBJECT_TIMEOUT_US                  (100000)
#endif

#define DEVICES_WATCHDOG_POWER_OFF_SLEEP_TIMEOUT (3600000)
#define WAIT_TRY_COUNT                           (10000)

#endif /* !defined(SYSINFO_H) */
