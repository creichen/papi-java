/*
 * Copyright (c) 2014 Charles University in Prague
 * Copyright (c) 2014 Vojtech Horky
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "papi_Wrapper.h"
#include "papijava.h"

#include <stdlib.h>

#include <papi.h>


JNIEXPORT jint JNICALL Java_papi_Wrapper_startCounters
		(JNIEnv *env, jobject UNUSED_ARG(self), jintArray eventsarr) {
	if (eventsarr == NULL) {
		return PAPI_EINVAL;
	}

	int events_count = (*env)->GetArrayLength(env, eventsarr);
	if (events_count == 0) {
		return PAPI_EINVAL;
	}

	jint *eventsj = (*env)->GetIntArrayElements(env, eventsarr, NULL);
	int *events = (int *) eventsj;

	int rc = PAPI_start_counters(events, events_count);

	(*env)->ReleaseIntArrayElements(env, eventsarr, eventsj, JNI_ABORT);

	return rc;
}

JNIEXPORT jint JNICALL Java_papi_Wrapper_stopCounters
		(JNIEnv *env, jobject UNUSED_ARG(self), jlongArray valuesarr) {
	if (valuesarr == NULL) {
		return PAPI_EINVAL;
	}

	int values_count = (*env)->GetArrayLength(env, valuesarr);
	if (values_count == 0) {
		return PAPI_EINVAL;
	}

	jlong *valuesj = (*env)->GetLongArrayElements(env, valuesarr, NULL);
	long long *values = (long long *) valuesj;

	int rc = PAPI_stop_counters(values, values_count);

	if (rc == PAPI_OK) {
		(*env)->ReleaseLongArrayElements(env, valuesarr, valuesj, JNI_COMMIT);
	} else {
		(*env)->ReleaseLongArrayElements(env, valuesarr, valuesj, JNI_ABORT);
	}

	return rc;
}
