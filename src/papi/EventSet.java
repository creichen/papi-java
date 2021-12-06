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

import java.util.Arrays;

public class EventSet {
	private long eventSetId = 0;
	private int events_nr;

	public static EventSet create(int... events) throws PapiException {
		EventSet set = new EventSet();
		set.events_nr = events.length;

		long id[] = new long[1];
		int rc = Wrapper.eventSetCreate(id);
		PapiException.throwOnError(rc, "creating event set");

		rc = Wrapper.eventSetAddEvents(id[0], events);
		PapiException.throwOnError(rc, "adding events to the set");
		set.eventSetId = id[0];

		return set;
	}

	private EventSet() {
	}

	public void destroy() {
		int rc = Wrapper.eventSetDestroy(eventSetId);
		if (rc != Constants.PAPI_OK) {
			throw new PapiRuntimeException(rc, "destroying event set");
		}
	}

	public void start() throws PapiException {
		int rc = Wrapper.eventSetStart(eventSetId);
		PapiException.throwOnError(rc, "starting event set");
	}

	/**
	 * Stops a running event set and reads performance counters
	 *
	 * @return The event counters, or null if the event hadn't started yet
	 */
	public long[] stop() throws PapiException {
		long[] results = new long[this.events_nr];
		int rc = Wrapper.eventSetStop(eventSetId, results);
		if (rc == Constants.PAPI_OK) {
			return results;
		}
		if (rc != Constants.PAPI_ENOTRUN) {
			PapiException.throwOnError(rc, "stopping");
		}
		return null;
	}

	/**
	 * Stops a running event set and reads the performance counters
	 *
	 * @param results The array to write the counters to
	 * @return true if the event set had been running
	 */
	public boolean stopAndRead(long[] results) throws PapiException {
		int rc = Wrapper.eventSetStop(eventSetId, results);
		if (rc == Constants.PAPI_OK) {
			return true;
		}
		if (rc != Constants.PAPI_ENOTRUN) {
			PapiException.throwOnError(rc, "stopping and reading");
		}
		return false;
	}

	public int size() {
		return this.events_nr;
	}

	/**
	 * Zeroes the event counters
	 */
	public void reset() {
		int rc = Wrapper.eventSetReset(this.eventSetId);
		PapiException.throwOnError(rc, "resetting");
	}

	/**
	 * Extracts the current counter values, adds them to the destination array, and zeroes the counter registers
	 *
	 * @param dest The destination array to add to
	 */
	public void addAndZero(long[] dest) {
		int rc = Wrapper.eventSetAccum(this.eventSetId, dest);
		PapiException.throwOnError(rc, "extracting, accumulating, and zeroing");
	}

	/**
	 * Extracts the current counter values
	 *
	 * The counters will keep running after this operation.
	 *
	 * @param dest The destination array to write to
	 */
	public void read(long[] dest) {
		int rc = Wrapper.eventSetRead(this.eventSetId, dest);
		PapiException.throwOnError(rc, "extracting, accumulating, and zeroing");
	}
}
