# PAPI for Java

This package is an update to Vojtech Horky's PAPI for Java bindings.
PAPI provides access to the CPU's hardware performance counters, which
are special CPU features that allow the CPU to measure low-level
events.  See [the PAPI homepage](https://icl.utk.edu/papi/index.html)
for more details.

## News
- JDK 9 and later supported out of the box
- PAPI 6 supported (by removing `papi.Wrapper.startCounters()` and `papi.Wrapper.stopCounters()`; use the EventSet API instead.)

## Dependencies

This package relies on:
- bash, GNU make, C/C++ build environment
- JDK 8 or later (tested with 8 and 11)
- Pre-existing PAPI installation on your system (PAPI 5 or 6 supported)
- user access to PAPI, e.g. by setting `/proc/sys/kernel/perf_event_paranoid` on Linux

## Building

You can build PAPI as follows:

bash:
```bash
./configure && make
```

fish:
```fish
./configure && make
```

The configuration script should auto-detect all relevant settings.  Otherwise you override by setting:
- `PAPI_PATH`: path under which we can find the `include` directory containing the PAPI headers (e.g., `/usr/`)
- `PAPI_LIBRARY_PATH`: path that contains `libpapi.so`
- `JAVAC`: which `javac` to use
- `JAVAH`: which JNI code generator to use: `javah` for JDK8, or `javac` for JDK 9 and later.

## Running

Try running the test:

```
make test
```

## Troubleshooting

* If `make test` doesn't find the `papi` library, adjust `PAPI_LIBRARY_PATH` to point to the directory that contains `libpapi.so`.
* If you get failures with error code -25, try setting `/proc/sys/kernel/perf_event_paranoid` to a lower value (such as 0).
* Migration: if you used an older version of this library and noticed that `papi.Wrapper.startCounters()` and `papi.Wrapper.stopCounters()` are missing: please migrate as follows:

```
papi.Wrapper.startCounters(COUNTERS);
...
papi.Wrapper.stopCounters(OUT);
```

should become

```
// if you were checking return codes before: the EventSet uses exceptions instead
EventSet s = papi.EventSet.create(COUNTERS);
s.start();
...
s.stop();
OUT = s.getCounters();
```


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
* Copyright (c) 2018-2021 Christoph Reichenbach

## TODO

- Expose more of the PAPI API
- JavaDoc
- Turn PAPI events into enum, so we can use `EnumSet.all()` to iterate over all events
- Make PAPI events self-documenting
- Use PapiRuntimeExceptions throughout
