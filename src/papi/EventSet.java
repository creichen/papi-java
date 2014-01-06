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
	private long eventId[] = new long[1];
	private long counters[];
	
	public static EventSet create(int... events) throws PapiException {
		EventSet set = new EventSet();
		set.counters = new long[events.length];
		
		int rc = Wrapper.eventSetCreate(set.eventId);
		PapiException.throwOnError(rc, "creating event set");
		
		rc = Wrapper.eventSetAddEvents(set.eventId[0], events);
		PapiException.throwOnError(rc, "adding events to the set");
		
		return set;
	}
	
	private EventSet() {
	}
	
	public void destroy() {
		int rc = Wrapper.eventSetDestroy(eventId[0]);
		if (rc != Constants.PAPI_OK) {
			throw new PapiRuntimeException(rc, "destroying event set");
		}
	}
	
	public void start() throws PapiException {
		int rc = Wrapper.eventSetStart(eventId[0]);
		PapiException.throwOnError(rc, "starting event set");
	}
	
	public void stop() throws PapiException {
		int rc = Wrapper.eventSetStop(eventId[0], counters);
		PapiException.throwOnError(rc, "stopping events");
	}
	
	public long[] getCounters() {
		return Arrays.copyOf(counters, counters.length);
	}
}
