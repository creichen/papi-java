# PAPI for Java

This package is an update to Vojtech Horky's PAPI for Java bindings.  It makes

## API

```Java
	EventSet evset = EventSet.create(Constants.PAPI_TOT_CYC, Constants.PAPI_L1_DCM);
	evset.start();
	// execute the code you want to measure
	evset.stop();
	long[] data = evset.getCounters();
```

##

Copyright (c) 2014 Charles University in Prague
Copyright (c) 2014 Vojtech Horky
Copyright (c) 2018 Christoph Reichenbach

* [PAPI performance counter description](https://icl.cs.utk.edu/projects/papi/wiki/PAPIC:Preset_Event_Definitions)
