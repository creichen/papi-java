#
# Copyright (c) 2014 Charles University in Prague
# Copyright (c) 2014 Vojtech Horky
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# - Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# - Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
# - The name of the author may not be used to endorse or promote products
#   derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

CC = gcc

CFLAGS = -Wall -Wextra -Werror -DPAPIJAVA_DEBUG
CFLAGS_JNI = -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux -fPIC
LDFLAGS = -lpapi

JAVA = java
JAVAC = javac
JAVAH = javah
JAR = jar

WRAPPER_CLASS = papi.Wrapper
WRAPPER_CLASS_JAVA_FILE = papi/Wrapper.java
WRAPPER_CLASS_C_HEADER = papi_Wrapper.h

JNI_OBJECTS = jni/highlevel.o jni/eventset.o jni/misc.o

JAVA_JUNIT_TESTS = papi.HighLevelApiTest papi.EventSetApiTest

PAPI_LIBRARY_PATH = ../

all: libpapijava.so papi.jar papi-tests.jar

libpapijava.so: $(JNI_OBJECTS)
	$(CC) $(CFLAGS) -o $@ -shared $(CFLAGS_JNI) $(JNI_OBJECTS) $(LDFLAGS)

papi.jar: src/papi/*.class
	$(JAR) cf $@ -C src .

papi-tests.jar: test/papi/*.class
	$(JAR) cf $@ -C test .

src/papi/%.class: src/papi/%.java
	$(JAVAC) -cp src src/papi/*.java

src/papi/*.class: src/papi/Constants.java

test/papi/%.class: papi.jar test/papi/%.java
	$(JAVAC) -cp test:papi.jar:lib/junit-4.11.jar:lib/hamcrest-core-1.3.jar test/papi/*.java

src/papi/Constants.java: jni/genconst.bin
	./jni/genconst.bin >$@

jni/$(WRAPPER_CLASS_C_HEADER): src/$(WRAPPER_CLASS_JAVA_FILE)
	$(JAVAH) -cp src/ -d jni/ $(WRAPPER_CLASS)

jni/*.c: jni/$(WRAPPER_CLASS_C_HEADER)

jni/%.o: jni/%.c
	$(CC) $(CFLAGS) -c -o $@ $(CFLAGS_JNI) $<

jni/genconst.bin: jni/genconst.o
	$(CC) -o $@ $(CFLAGS) -lpapi $< $(LDFLAGS)

run-junit-tests: papi-tests.jar papi.jar libpapijava.so
	$(JAVA) \
		-cp papi.jar:papi-tests.jar:lib/junit-4.11.jar:lib/hamcrest-core-1.3.jar \
		-Djava.library.path=.:$(PAPI_LIBRARY_PATH) \
		org.junit.runner.JUnitCore \
		$(JAVA_JUNIT_TESTS)

clean:
	rm -f \
		*.jar src/papi/*.class test/papi/*.class \
		jni/*.o *.so jni/genconst.bin \
		jni/papi_Wrapper.h src/papi/Constants.java
