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

public class PapiException extends RuntimeException {
        private static final long serialVersionUID = 12361391L;

        public static final String UNKNOWN_PAPI_ERROR_MESSAGE = "<UNKNOWN>";

        private int papi_error_code;

        public PapiException(final int rc, final String reason) {
                super(reason + " [error code = " + rc + ", " + PapiException.papiErrorMessage(rc) + "]");
        }

        public int
        getPapiErrorCode() {
                return this.papi_error_code;
        }

        public static String
        papiErrorMessage(final int papi_error_code) {
                if (Constants.ERRORS.containsKey(papi_error_code)) {
                        return Constants.ERRORS.get(papi_error_code);
                }
                return PapiException.UNKNOWN_PAPI_ERROR_MESSAGE;
        }

        public String
        getPapiErrorMessage() {
                return PapiException.papiErrorMessage(this.getPapiErrorCode());
        }

        public static void throwOnError(final int rc, final String reason) throws PapiException {
                if (rc != Constants.PAPI_OK) {
                        throw new PapiException(rc, reason);
                }
        }
}
