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

#include <asm/unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#include "perf-events.h"

// map HW_PERF_CLASS_CPU_CPU_* to perf events
static int hw_perf_class_cpu_events[HW_PERF_CLASS_CPU_EVENTS_SIZE] = {
	PERF_COUNT_HW_CPU_CYCLES,
	PERF_COUNT_HW_INSTRUCTIONS,
	PERF_COUNT_HW_CACHE_REFERENCES,
	PERF_COUNT_HW_CACHE_MISSES,
	PERF_COUNT_HW_BRANCH_INSTRUCTIONS,
	PERF_COUNT_HW_BRANCH_MISSES,
	PERF_COUNT_HW_BUS_CYCLES,
	PERF_COUNT_HW_STALLED_CYCLES_FRONTEND,
	PERF_COUNT_HW_STALLED_CYCLES_BACKEND,
	PERF_COUNT_HW_REF_CPU_CYCLES
};

// Combine the three tables below to map HW_PERF_CLASS_CACHE_* to perf events
static int hw_perf_class_caches[HW_PERF_CLASS_CACHES_SIZE] = {
	PERF_COUNT_HW_CACHE_L1D,
	PERF_COUNT_HW_CACHE_L1I,
	PERF_COUNT_HW_CACHE_LL,
	PERF_COUNT_HW_CACHE_DTLB,
	PERF_COUNT_HW_CACHE_ITLB,
	PERF_COUNT_HW_CACHE_BPU,
	PERF_COUNT_HW_CACHE_NODE
};

static int hw_perf_class_cacheops[HW_PERF_CLASS_CACHEOPS_SIZE] = {
	PERF_COUNT_HW_CACHE_OP_READ << 8,
	PERF_COUNT_HW_CACHE_OP_WRITE << 8,
	PERF_COUNT_HW_CACHE_OP_PREFETCH << 8
};

static int hw_perf_class_cacheresults[HW_PERF_CLASS_CACHERESULTS_SIZE] = {
	PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16,
	PERF_COUNT_HW_CACHE_RESULT_MISS << 16
};


// ----------------------------------------
// strings
static char* hw_perf_class_cpu_events_str[HW_PERF_CLASS_CPU_EVENTS_SIZE] = {
	"CYCLES",
	"INSNS",
	"CACHE_REFS",
	"CACHE_MISSES",
	"BRANCH_INSNS",
	"BRANCH_MISSES",
	"BUS_CYCLES",
	"STALLED_CYC_FE",
	"STALLED_CYC_BE",
	"CYCLES_REF"
};

static char* hw_perf_class_caches_str[HW_PERF_CLASS_CACHES_SIZE] = {
	"L1D",
	"L1I",
	"LL",
	"DTLB",
	"ITLB",
	"BPU",
	"NODE",
};

static char* hw_perf_class_cacheops_str[HW_PERF_CLASS_CACHEOPS_SIZE] = {
	"RD",
	"WR",
	"PF",
};

static char* hw_perf_class_cacheresults_str[HW_PERF_CLASS_CACHERESULTS_SIZE] = {
	"TOTAL",
	"MISS"
};


int
hwperf_event_name(char* output, unsigned int event_id)
{
	strcpy(output, "invalid-id");

	const int ty = (event_id >> HW_PERF_TY_SHIFT) & HW_PERF_MASK;
	const int cls = (event_id >> HW_PERF_CLASS_SHIFT) & HW_PERF_MASK;
	const int cacheop = (event_id >> HW_PERF_CACHEOP_SHIFT) & HW_PERF_MASK;
	const int cacheresult = (event_id >> HW_PERF_CACHERESULT_SHIFT) & HW_PERF_MASK;

	switch (ty) {
	case HW_PERF_TY_CPU:
		if (cls >= HW_PERF_CLASS_CPU_EVENTS_SIZE) {
			return 1;
		}
		strcpy(output, hw_perf_class_cpu_events_str[cls]);
		return 0;

	case HW_PERF_TY_CACHE:
		if (cls >= HW_PERF_CLASS_CACHES_SIZE
		    || cacheop >= HW_PERF_CLASS_CACHEOPS_SIZE
		    || cacheresult >= HW_PERF_CLASS_CACHERESULTS_SIZE) {
			return 1;
		}
		strcpy(output, "C_");
		strcat(output, hw_perf_class_caches_str[cls]);
		strcat(output, "_");
		strcat(output, hw_perf_class_cacheops_str[cacheop]);
		strcat(output, "_");
		strcat(output, hw_perf_class_cacheresults_str[cacheresult]);
		return 0;

	default:
		return 1;
	}
}

int
hwperf_set_config(struct perf_event_attr* event, unsigned int event_id)
{
	const int ty = (event_id >> HW_PERF_TY_SHIFT) & HW_PERF_MASK;
	const int cls = (event_id >> HW_PERF_CLASS_SHIFT) & HW_PERF_MASK;
	const int cacheop = (event_id >> HW_PERF_CACHEOP_SHIFT) & HW_PERF_MASK;
	const int cacheresult = (event_id >> HW_PERF_CACHERESULT_SHIFT) & HW_PERF_MASK;

	switch (ty) {
	case HW_PERF_TY_CPU:
		if (cls >= HW_PERF_CLASS_CPU_EVENTS_SIZE) {
			return 1;
		}
		event->type = PERF_TYPE_HARDWARE;
		event->config = hw_perf_class_cpu_events[cls];
		return 0;

	case HW_PERF_TY_CACHE:
		if (cls >= HW_PERF_CLASS_CACHES_SIZE
		    || cacheop >= HW_PERF_CLASS_CACHEOPS_SIZE
		    || cacheresult >= HW_PERF_CLASS_CACHERESULTS_SIZE) {
			return 1;
		}
		event->type = PERF_TYPE_HW_CACHE;
		event->config = hw_perf_class_caches[cls]
			| hw_perf_class_cacheops[cacheop]
			| hw_perf_class_cacheresults[cacheresult];
		return 0;

	default:
		return 1;
	}
}

static int
perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags)
{
	return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}



int
hwperf_open(unsigned int event_id) {
	struct perf_event_attr pe;

	memset(&pe, 0, sizeof(struct perf_event_attr));
	hwperf_set_config(&pe, event_id);
	pe.disabled = 1;
	pe.exclude_kernel = 1;
	pe.exclude_hv = 1;

	int fd = perf_event_open(&pe, 0, -1, -1, 0);
	if (fd == -1) {
		return errno;
	}
	if (hwperf_reset(fd)) {
		close(fd);
		return -ENODEV;
	}
	return fd;

}

long long
hwperf_read(int fd) {
	long long result;
	int ret = read(fd, &result, sizeof(long long));
	if (ret < 0) {
		perror("reading perfcounter status");
		return 0ll;
	}
	return result;
}
