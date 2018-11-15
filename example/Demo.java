/*
 * Copyright (c) 2018 Christoph Reichenbach
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

package example;
import papi.*;

public class Demo {

    public static void main(String[] args) throws PapiException {
	Papi.init();

	EventSet evset = EventSet.create(Constants.PAPI_TOT_CYC, Constants.PAPI_L1_DCM);

	int[] results = new int[10];

	// 9 warmup runs before measuring
	for (int warmup = 10; warmup >= 0; --warmup) {
	    for (int i = 0; i < 10; i++) {
		evset.start();

		// some weird code to measure
		for (int k = 0; k <= i*10; k++) {
		    results[i] += k*k;
		}
		// done with the code

		evset.stop();
		long[] data = evset.getCounters();

		// only print the 10th run
		if (warmup == 0) {
		    System.out.println("#" + i + ":\t" + data[0] + "\t" + data[1]);
		}
	    }
	}
    }
}
