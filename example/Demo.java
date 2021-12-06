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
import java.util.*;
import papi.*;

public class Demo {
    public static final int[] COUNTERS = new int[] { Constants.PAPI_TOT_CYC, Constants.PAPI_L1_DCM };
    public static final int DEMO_SIZE = 16384;
    public static final String L1_DCM_EXPECTED = "(expected: ca. 32768 on 32k L1D, ca. 16384 on 48k L1D)";

    /**
     * If cycle == 0, print numbers
     */
    public static void printIfDone(int warmup, String prefix, long[] counters) {
	if (warmup == 0) {
	    System.out.println(prefix + ":\n\t cycles:\t" + counters[0] + "\tl1-dcm:\t" + counters[1]);
	}
    }

    public static void add(long[] dest, long[] src) {
	for (int i =0 ; i < dest.length; ++i) {
	    dest[i] += src[i];
	}
    }

    public static void sub(long[] dest, long[] src) {
	for (int i = 0 ; i < dest.length; ++i) {
	    dest[i] -= src[i];
	}
    }

    public static int[]
    initReflist(int offset) {
	int[] reflist = new int[DEMO_SIZE];
	ArrayList<Integer> unused_locations = new ArrayList<Integer>();
	for (int i = 0; i < DEMO_SIZE; ++i) {
	    unused_locations.add(i);
	}
	Collections.shuffle(unused_locations, new Random(offset));

	int start = unused_locations.get(0);
	int pos = start;
	for (int i = 1; i < unused_locations.size(); ++i) {
	    int next = unused_locations.get(i);
	    reflist[pos] = next;
	    pos = next;
	}
	reflist[pos] = start;
	//System.err.println(Arrays.toString(reflist));
	return reflist;
    }

    public static void testStop() {
	EventSet evset = EventSet.create(COUNTERS);
	long[] total_counters = new long[2];

	// warmup runs before measuring
	for (int warmup = 100; warmup >= 0; --warmup) {
	    int[] reflist =  Demo.initReflist(warmup);

	    evset.start();

	    int pos = 0;
	    int count = 0;
	    do {
		++count;
		if (count > DEMO_SIZE + 1) {
		    break;
		}
		pos = reflist[pos];
	    } while (pos != 0);

	    if (count != DEMO_SIZE) {
		throw new RuntimeException("Cache maze failed");
	    }

	    long[] data = evset.stop();
	    if (warmup < 4) {
		add(total_counters, data);
	    }

	    // only print the last run
	    printIfDone(warmup, "stop()", total_counters);
	}
	evset.destroy();
    }

    public static void testStopAndRead() {
	EventSet evset = EventSet.create(COUNTERS);
	long[] total_counters = new long[2];
	long[] data = new long[2];

	// warmup runs before measuring
	for (int warmup = 100; warmup >= 0; --warmup) {
	    int[] reflist =  Demo.initReflist(warmup);

	    evset.start();

	    int pos = 0;
	    int count = 0;
	    do {
		++count;
		if (count > DEMO_SIZE + 1) {
		    break;
		}
		pos = reflist[pos];
	    } while (pos != 0);

	    if (count != DEMO_SIZE) {
		throw new RuntimeException("Cache maze failed");
	    }

	    evset.stopAndRead(data);
	    if (warmup < 4) {
		add(total_counters, data);
	    }

	    // only print the last run
	    printIfDone(warmup, "stopAndRead()", total_counters);
	}
	evset.destroy();
    }

    public static void testAddAndZero() {
	EventSet evset = EventSet.create(COUNTERS);
	long[] total_counters = new long[2];
	long[] dummy = new long[2];

	evset.start();

	// warmup runs before measuring
	for (int warmup = 100; warmup >= 0; --warmup) {
	    int[] reflist =  Demo.initReflist(warmup);

	    evset.addAndZero(dummy);

	    int pos = 0;
	    int count = 0;
	    do {
		++count;
		if (count > DEMO_SIZE + 1) {
		    break;
		}
		pos = reflist[pos];
	    } while (pos != 0);

	    if (count != DEMO_SIZE) {
		throw new RuntimeException("Cache maze failed");
	    }

	    if (warmup < 4) {
		evset.addAndZero(total_counters);
	    }

	    // only print the last run
	    printIfDone(warmup, "addAndZero()", total_counters);
	}
	evset.stop();
	evset.destroy();
    }

    public static void testResetAddAndZero() {
	EventSet evset = EventSet.create(COUNTERS);
	long[] total_counters = new long[2];

	evset.start();

	// warmup runs before measuring
	for (int warmup = 100; warmup >= 0; --warmup) {
	    int[] reflist =  Demo.initReflist(warmup);

	    evset.reset();

	    int pos = 0;
	    int count = 0;
	    do {
		++count;
		if (count > DEMO_SIZE + 1) {
		    break;
		}
		pos = reflist[pos];
	    } while (pos != 0);

	    if (count != DEMO_SIZE) {
		throw new RuntimeException("Cache maze failed");
	    }

	    if (warmup < 4) {
		evset.addAndZero(total_counters);
	    }

	    // only print the last run
	    printIfDone(warmup, "reset()+addAndZero()", total_counters);
	}
	evset.stop();
	evset.destroy();
    }

    public static void testRead() {
	EventSet evset = EventSet.create(COUNTERS);
	long[] total_counters = new long[2];
	long[] start_counters = new long[2];
	long[] stop_counters = new long[2];

	evset.start();

	// warmup runs before measuring
	for (int warmup = 100; warmup >= 0; --warmup) {
	    int[] reflist =  Demo.initReflist(warmup);

	    evset.read(start_counters);

	    int pos = 0;
	    int count = 0;
	    do {
		++count;
		if (count > DEMO_SIZE + 1) {
		    break;
		}
		pos = reflist[pos];
	    } while (pos != 0);

	    if (count != DEMO_SIZE) {
		throw new RuntimeException("Cache maze failed");
	    }

	    if (warmup < 4) {
		evset.read(stop_counters);
		sub(stop_counters, start_counters);
		add(total_counters, stop_counters);
	    }

	    // only print the last run
	    printIfDone(warmup, "read()", total_counters);
	}
	evset.stop();
	evset.destroy();
    }

    public static void printSupportedCounters() {
	ArrayList<String> names = new ArrayList<>();
	int failed = 0;
	for (String ev_name : Constants.EVENTS.keySet()) {
	    int ev_id = Constants.EVENTS.get(ev_name);
	    try {
		EventSet evset = EventSet.create(ev_id);
		evset.start();
		evset.stop();
		evset.destroy();
		names.add(ev_name);
	    } catch (PapiException e) {
		failed += 1;
	    }
	}
	Collections.sort(names);
	System.out.println("Supported events:");
	for (String n : names) {
	    System.out.println("\t" + n);
	}
	System.out.println("(" + failed + " event(s) unsupported)\n");
    }

    public static void main(String[] args) throws PapiException {
	Papi.init();

	printSupportedCounters();

	testStop();
	testStopAndRead();
	testAddAndZero();
	testResetAddAndZero();
	testRead();
    }
}
