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
package papi;

import org.junit.BeforeClass;
import org.junit.Test;

import static org.junit.Assert.*;

public class EventSetApiTest {
        @BeforeClass
        public static void initialization() {
                Papi.init();
        }

        @Test
        public void nullArgumentForCreatingEventSetFails() {
                assertEquals(Constants.PAPI_EINVAL, Wrapper.eventSetCreate(null));
        }

        @Test
        public void emptyArrayArgumentForCreatingEventSetFails() {
                assertEquals(Constants.PAPI_EINVAL, Wrapper.eventSetCreate(new long[0]));
        }

        @Test
        public void tooBigArrayArgumentForCreatingEventSetFails() {
                assertEquals(Constants.PAPI_EINVAL, Wrapper.eventSetCreate(new long[2]));
        }

        @Test
        public void testReset() {
                EventSet ev = EventSet.create(Constants.PAPI_TOT_CYC);
                ev.start();
                ev.stop();
                ev.reset();
                ev.start();
                ev.stop();
        }

        @Test
        public void testTimeSpentCounting() {
                int iterations = (int) 1e6;

                EventSet ev = EventSet.create(Constants.PAPI_TOT_CYC);

                long[] counterValues = new long[iterations];

                for (int i = 0; i < iterations; ++i) {
                        ev.reset();
                        ev.start();

                        long[] readings = ev.stop();
                        counterValues[i] = readings[0];
                }

                // We compute the average.
                long sum = 0;
                for (int i = 0; i < iterations; ++i) {
                        sum += counterValues[i];
                }

                assertTrue((float) sum / iterations > 10.0);
        }
}
