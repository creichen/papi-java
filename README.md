# PAPI for Java

This package is an update to Vojtech Horky's PAPI for Java bindings.
PAPI provides access to the CPU's hardware performance counters, which
are special CPU features that allow the CPU to measure low-level
events.  See [the PAPI homepage](https://icl.utk.edu/papi/index.html)
for more details.

## Dependencies

This package relies on a pre-existing installation of PAPI on your system.

## Building

You can build PAPI as follows:

```bash
PAPI_PATH=/usr/ make
```

where `PAPI_PATH` points to your PAPI installation path (used to find
the PAPI includes).  Note that the Makefile will execute C code to
query PAPI for some values that are then transformed into Java source
code.

## Running

Try running the tests:

```
PAPI_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu/ make test
```

* If this doesn't find the `papi` library, adjust `PAPI_LIBRARY_PATH` to point to the directory that contains `libpapi.so`.
* If you get failures with error code -25, try setting `/proc/sys/kernel/perf_event_paranoid` to a lower value (such as 0).

## API

```Java
	Papi.init();

	EventSet evset = EventSet.create(Constants.PAPI_TOT_CYC, Constants.PAPI_L1_DCM);
	evset.start();
	// execute the code you want to measure
	evset.stop();
	long[] data = evset.getCounters();
```

See also `example/Demo.java` for a full demo.

The [PAPI performance counter documentation](https://icl.cs.utk.edu/projects/papi/wiki/PAPIC:Preset_Event_Definitions) is very helpful to understand what it is that you are measuring.


## Copyright

Copyright (c) 2014 Charles University in Prague
Copyright (c) 2014 Vojtech Horky
Copyright (c) 2018 Christoph Reichenbach

##
