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
import java.math.BigInteger;

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

        public static BigInteger
        fib(int n) {
                if (n == 0) {
                        return BigInteger.ZERO;
                } else if (n == 1) {
                        return BigInteger.ONE;
                }
                return fib(n - 1).add(fib(n - 2));
        }


        @Test
        public void
        checkCyclesIncrement() throws PapiException {
                EventSet evset = EventSet.create(Constants.PAPI_TOT_CYC);
                long[] data = new long[1];

                final BigInteger[] expected = new BigInteger[] {
                        BigInteger.valueOf(0l),
                        BigInteger.valueOf(1l),
                        BigInteger.valueOf(1l),
                        BigInteger.valueOf(2l),
                        BigInteger.valueOf(3l),
                        BigInteger.valueOf(5l),
                        BigInteger.valueOf(8l),
                        BigInteger.valueOf(13l)
                };
                long[] counts = new long[expected.length];

                evset.start();
                for (int i = 1; i < expected.length; ++i) {
                        assertEquals(fib(i), expected[i]);
                        evset.read(data);
                        counts[i] = data[0];
                }

                for (int i = 2; i < expected.length; ++i) {
                        assertTrue(counts[i - 1] < counts[i]);
                }

                long[] result = evset.stop();
                assertTrue(counts[counts.length - 1] < result[0]);
        }
}
