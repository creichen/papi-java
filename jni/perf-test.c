#include <stdio.h>
#include <stdlib.h>
#include "perf-events.h"

#define SIZE 4096
int data[SIZE + 2];

int
workload(void) {
	data[SIZE] = 1;
	data[SIZE + 1] = 1;
	for (int i = 0; i < SIZE; ++i) {
		if (data[i] & 1) {
			data[i] += data[i + 1];
		} else {
			data[i] += 3;
		}
	}
}

int
try_out(int count, unsigned int event, char *descr) {
	int fd = hwperf_open(event);
	int err = 0;
	if (fd < 0) {
		perror(descr);
		return -1;
	}
	err = hwperf_reset(fd);
	if (err != 0) {
		perror(descr);
		fprintf(stderr, "failed to reset");
		return -1;
	}
	err = hwperf_enable(fd);
	if (err != 0) {
		perror(descr);
		fprintf(stderr, "failed to enable");
		return -1;
	}
	for (int i = 0; i < count; ++i) {
		workload();
	}
	err = hwperf_disable(fd);
	if (err != 0) {
		perror(descr);
		fprintf(stderr, "failed to disable");
		return -1;
	}
	printf("%s: %lld\n", descr, hwperf_read(fd));
	return 0;
}

int
main(int argc, char **argv) {
	int count = 1;
	if (argc > 1) {
		atoi(argv[1]);
	}

	try_out(count,
		HW_PERF_EVENT(HW_PERF_TY_CPU, HW_PERF_CLASS_CPU_INSTRUCTIONS, 0, 0),
		"insns");

	try_out(count,
		HW_PERF_EVENT(HW_PERF_TY_CPU, HW_PERF_CLASS_CPU_CACHE_MISSES, 0, 0),
		"cache-misses");

	try_out(count,
		HW_PERF_EVENT(HW_PERF_TY_CPU, HW_PERF_CLASS_CPU_BRANCH_INSNS, 0, 0),
		"branch-insns");

	try_out(count,
		HW_PERF_EVENT(HW_PERF_TY_CPU, HW_PERF_CLASS_CPU_BRANCH_MISSES, 0, 0),
		"branch-misses");

	try_out(count,
		HW_PERF_EVENT(HW_PERF_TY_CACHE, HW_PERF_CLASS_CACHE_L1D, HW_PERF_CACHEOP_READ, HW_PERF_CACHERESULT_MISS),
		"l1d-read-misses");

	return 0;
}
