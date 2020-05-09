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


#include <stdio.h>
#include <unistd.h>

#include "perf-events.h"

int count = 0;

void
try_event(unsigned int event_id) {
	int fd = hwperf_open(event_id);
	if (fd >= 0) {
		if (!hwperf_reset(fd) && !hwperf_enable(fd) && !hwperf_disable(fd)) {
			char namebuf[16];
			hwperf_event_name(namebuf, event_id);
			printf("%s\t0x%08x\n", namebuf, event_id);
			++count;
		}
		close(fd);
	}
}

int
main(int argc, char** argv) {
	for (int i = 0; i < HW_PERF_CLASS_CPU_EVENTS_SIZE; ++i) {
		try_event(HW_PERF_EVENT(HW_PERF_TY_CPU, i, 0, 0));
	}
	for (int cache_id = 0; cache_id < HW_PERF_CLASS_CACHES_SIZE; ++cache_id) {
		for (int cache_op = 0; cache_op < HW_PERF_CLASS_CACHEOPS_SIZE; ++cache_op) {
			for (int cache_result = 0; cache_result < HW_PERF_CLASS_CACHERESULTS_SIZE; ++cache_result) {
				try_event(HW_PERF_EVENT(HW_PERF_TY_CACHE, cache_id, cache_op, cache_result));
			}
		}
	}
	if (count == 0) {
		perror("last performance counter read");
		fprintf(stderr, "No available events.");
	}
	return count == 0;
}
