# Pseudo-PAPI for Java

This package is an alternative to Vojtech Horky's PAPI for Java bindings.

## Dependencies

This package requires a Linux kernel with `/proc/sys/kernel/perf_event_paranoid` set suitably low, e.g.,
```echo -1 > /proc/sys/kernel/perf_event_paranoid
```

## Building

Make sure that your `JAVA_HOME` is set appropriately (e.g., `/usr/lib/jvm/java-1.8.0-openjdk-arm64`).

You can build PAPI as follows:

```bash
make
```

Note that the Makefile will execute C code to query your system's
performance counter kernel API for values that are then transformed
into Java source code.

## Running

Try running the tests:

```bash
make test
```

* If you get failures, try setting `/proc/sys/kernel/perf_event_paranoid` to a lower value (such as -1).

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

* Copyright (c) 2014 Charles University in Prague
* Copyright (c) 2014 Vojtech Horky
* Copyright (c) 2018--2020 Christoph Reichenbach

## TODO

- Expose more of the PAPI API
- JavaDoc
- Turn PAPI events into enum, so we can use `EnumSet.all()` to iterate over all events
- Make PAPI events self-documenting
- Use PapiRuntimeExceptions throughout
- Allow event sets with multiple events again
