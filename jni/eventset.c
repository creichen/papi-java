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
#include "papi_Wrapper.h"
#include "papijava.h"

#include <stdlib.h>

#include "perf-events.h"

JNIEXPORT jint JNICALL Java_papi_Wrapper_readCounter
		(JNIEnv *env, jclass UNUSED_ARG(self), jint fd, jlongArray valuesarr) {
	if (valuesarr == NULL) {
		return PAPI_EINVAL;
	}

	int values_count = (*env)->GetArrayLength(env, valuesarr);
	if (values_count != 1) {
		return PAPI_EINVAL;
	}

	long long count = hwperf_read((int) fd);

	jlong *valuesj = (*env)->GetLongArrayElements(env, valuesarr, NULL);
	valuesj[0] = (jlong) count;
	(*env)->ReleaseLongArrayElements(env, valuesarr, valuesj, JNI_COMMIT);

	return PAPI_OK;
}
