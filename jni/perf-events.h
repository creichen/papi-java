/***************************************************************************
  Copyright (C) 2020 Christoph Reichenbach


 This program may be modified and copied freely according to the terms
 of the GNU general public license (GPL), verison 3, as long as the
 above copyright notice and the licensing information contained herein
 are preserved.

 Please refer to www.gnu.org for licensing details.

 This work is provided AS IS, without warranty of any kind, expressed or
 implied, including but not limited to the warranties of merchantability,
 noninfringement, and fitness for a specific purpose. The author will not
 be held liable for any damage caused by this work or derivatives of it.

 By using this source code, you agree to the licensing terms as stated
 above.


 Please contact the maintainer for bug reports or inquiries.

 Current Maintainer:

    Christoph Reichenbach (CR) <creichen@gmail.com>

***************************************************************************/


#ifndef _PAPIJAVA_PERF_EVENTS_H
#define _PAPIJAVA_PERF_EVENTS_H

#include <linux/perf_event.h>
#include <sys/ioctl.h>

// fake PAPI version
#define PAPI_VER_CURRENT 0xff000001

// fake PAPI API
#define PAPI_OK 0
#define PAPI_EINVAL -1

// perf events and translation table from the pseudo-PAPI event IDs

#define HW_PERF_EVENT(type, class, cacheop, result)	\
	(((type) << HW_PERF_TY_SHIFT)			\
     | ((class) << HW_PERF_CLASS_SHIFT)			\
     | ((cacheop) << HW_PERF_CACHEOP_SHIFT)		\
     | ((result) << HW_PERF_CACHERESULT_SHIFT))


#define HW_PERF_MASK	0xff

// most significant byte
#define HW_PERF_TY_SHIFT	24
#define HW_PERF_TY_INVALID	0x00
#define HW_PERF_TY_CPU		0x01
#define HW_PERF_TY_CACHE	0x02

// classes further specialise
#define HW_PERF_CLASS_SHIFT	16
#define HW_PERF_CLASS_CPU_CPU_CYCLES			0x00
#define HW_PERF_CLASS_CPU_INSTRUCTIONS			0x01
#define HW_PERF_CLASS_CPU_CACHE_REFERENCES		0x02
#define HW_PERF_CLASS_CPU_CACHE_MISSES			0x03
#define HW_PERF_CLASS_CPU_BRANCH_INSNS			0x04
#define HW_PERF_CLASS_CPU_BRANCH_MISSES			0x05
#define HW_PERF_CLASS_CPU_BUS_CYCLES			0x06
#define HW_PERF_CLASS_CPU_STALLED_CYCLES_FRONTEND	0x07
#define HW_PERF_CLASS_CPU_STALLED_CYCLES_BACKEND	0x08
#define HW_PERF_CLASS_CPU_REF_CPU_CYCLES		0x09
#define HW_PERF_CLASS_CACHE_L1D		0x00
#define HW_PERF_CLASS_CACHE_L1I		0x01
#define HW_PERF_CLASS_CACHE_LL		0x02
#define HW_PERF_CLASS_CACHE_DTLB	0x03
#define HW_PERF_CLASS_CACHE_ITLB	0x04
#define HW_PERF_CLASS_CACHE_BPU		0x05
#define HW_PERF_CLASS_CACHE_NODE	0x06


#define HW_PERF_CACHEOP_SHIFT	8
#define HW_PERF_CACHEOP_READ		0x00
#define HW_PERF_CACHEOP_WRITE		0x01
#define HW_PERF_CACHEOP_PREFETCH	0x02

#define HW_PERF_CACHERESULT_SHIFT	0
#define HW_PERF_CACHERESULT_ACCESS	0x00
#define HW_PERF_CACHERESULT_MISS	0x01


#define HW_PERF_CLASS_CPU_EVENTS_SIZE 10
#define HW_PERF_CLASS_CACHES_SIZE 7
#define HW_PERF_CLASS_CACHEOPS_SIZE 3
#define HW_PERF_CLASS_CACHERESULTS_SIZE 2

/**
 * Write event name to string buffer of at least 16 bytes
 */
int
hwperf_event_name(char* output, unsigned int event_id);

/**
 * Mark event type and config attributes as demanded by the event ID
 */
int
hwperf_set_config(struct perf_event_attr* event, unsigned int event_id);


/**
 * Returns a file descriptor for hwperf_start(), hwperf_pause(), hwperf_reset(), hwperf_read(), and close().
 *
 * Result is negative on error, such as:
 *   -ENODEV: event not supported
 */
int
hwperf_open(unsigned int event_id);

long long
hwperf_read(int fd);

#define hwperf_enable(fd) ioctl((fd), PERF_EVENT_IOC_ENABLE, 0)
#define hwperf_disable(fd) ioctl((fd), PERF_EVENT_IOC_DISABLE, 0)
#define hwperf_reset(fd) ioctl((fd), PERF_EVENT_IOC_RESET, 0)


#endif // !defined(_PAPIJAVA_PERF_EVENTS_H)
