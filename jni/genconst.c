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


#define JAVA_HEADER "/*\n" \
	" * This is generated file. Do not edit.\n" \
	" * To change this file, see the jni/genconst.c script and run\n" \
	" * make afterwards.\n" \
	" */\n\n" \
	"package papi;\n" \
	"\n" \
	"public class Constants {"

#define JAVA_FOOTER "}"

#define START_GROUP(description) \
	printf("\n\t/* %s */\n", description)

#define PRINT_CONST(name) \
	printf("\tpublic static final int %s = %d;\n", #name, name);

#define PRINT_CONSTX(name) \
	printf("\tpublic static final int %s = 0x%x;\n", #name, name);

int count = 0;

void
try_event(unsigned int event_id) {
	int fd = hwperf_open(event_id);
	if (fd >= 0) {
		if (!hwperf_reset(fd) && !hwperf_enable(fd) && !hwperf_disable(fd)) {
			char namebuf[16];
			hwperf_event_name(namebuf, event_id);
			printf("\tpublic static final int %s = 0x%x;\n", namebuf, event_id);
			++count;
		}
		close(fd);
	}
}

int main() {
	printf("%s\n", JAVA_HEADER);

	PRINT_CONSTX(PAPI_VER_CURRENT);

	START_GROUP("Error codes");
	PRINT_CONST(PAPI_OK);
	PRINT_CONST(PAPI_EINVAL);

	START_GROUP("Preset events");
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

	printf("%s\n", JAVA_FOOTER);

	return count == 0; // error if no events found
}
