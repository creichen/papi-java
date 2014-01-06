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

JNIEXPORT jint JNICALL Java_papi_Wrapper_eventSetCreate
		(JNIEnv *env, jclass UNUSED_ARG(self), jlongArray eventsetidoutarr) {
	if (eventsetidoutarr == NULL) {
		return PAPI_EINVAL;
	}

	if ((*env)->GetArrayLength(env, eventsetidoutarr) != 1) {
		return PAPI_EINVAL;
	}

	int eventset = PAPI_NULL;
	int rc = PAPI_create_eventset(&eventset);
	if (rc != PAPI_OK) {
		return rc;
	}

	jlong *eventsetidoutj = (*env)->GetLongArrayElements(env, eventsetidoutarr, NULL);
	long long *eventsetidout = (long long *) eventsetidoutj;
	eventsetidout[0] = eventset;
	(*env)->ReleaseLongArrayElements(env, eventsetidoutarr, eventsetidoutj, JNI_COMMIT);

	DEBUG_PRINT("new eventset is %d", eventset);

	return PAPI_OK;
}

JNIEXPORT jint JNICALL Java_papi_Wrapper_eventSetDestroy
		(JNIEnv UNUSED_ARG(*env), jclass UNUSED_ARG(self), jlong eventsetid) {
	int eventset = (int) eventsetid;

	DEBUG_PRINT("cleanig-up the event set %d first", eventset);
	int rc = PAPI_cleanup_eventset(eventset);
	if (rc != PAPI_OK) {
		goto leave;
	}

	DEBUG_PRINT("destroying %d", eventset);
	rc = PAPI_destroy_eventset(&eventset);

leave:
	DEBUG_PRINT("returning %d (%s)", rc, PAPI_strerror(rc));
	return rc;
}

JNIEXPORT jint JNICALL Java_papi_Wrapper_eventSetAddEvents
		(JNIEnv *env, jclass UNUSED_ARG(self), jlong eventsetid, jintArray eventsarr) {
	DEBUG_PRINT("enter");
	if (eventsarr == NULL) {
		DEBUG_PRINT("events array is NULL!");
		return PAPI_EINVAL;
	}

	int events_count = (*env)->GetArrayLength(env, eventsarr);
	if (events_count == 0) {
		return PAPI_OK;
	}

	int eventset = (int) eventsetid;
	DEBUG_PRINT("eventset is %d", eventset);

	jint *eventsj = (*env)->GetIntArrayElements(env, eventsarr, NULL);
	int *events = (int *) eventsj;
	int rc = PAPI_add_events(eventset, events, events_count);
	(*env)->ReleaseIntArrayElements(env, eventsarr, eventsj, JNI_ABORT);

	DEBUG_PRINT("returning %d (%s)", rc, PAPI_strerror(rc));

	return rc;
}

JNIEXPORT jint JNICALL Java_papi_Wrapper_eventSetStart
		(JNIEnv UNUSED_ARG(*env), jclass UNUSED_ARG(self), jlong eventsetid) {
	int eventset = (int) eventsetid;
	return PAPI_start(eventset);
}

JNIEXPORT jint JNICALL Java_papi_Wrapper_eventSetStop
		(JNIEnv *env, jclass UNUSED_ARG(self), jlong eventsetid, jlongArray valuesarr) {
	if (valuesarr == NULL) {
		return PAPI_EINVAL;
	}

	int values_count = (*env)->GetArrayLength(env, valuesarr);
	if (values_count == 0) {
		return PAPI_EINVAL;
	}

	int eventset = (int) eventsetid;

	jlong *valuesj = (*env)->GetLongArrayElements(env, valuesarr, NULL);
	long long *values = (long long *) valuesj;

	int rc = PAPI_stop(eventset, values);

	if (rc == PAPI_OK) {
		(*env)->ReleaseLongArrayElements(env, valuesarr, valuesj, JNI_COMMIT);
	} else {
		(*env)->ReleaseLongArrayElements(env, valuesarr, valuesj, JNI_ABORT);
	}

	DEBUG_PRINT("returning %d (%s)", rc, PAPI_strerror(rc));

	return rc;
}
