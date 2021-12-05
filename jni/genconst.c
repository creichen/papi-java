/*
 * Copyright (c) 2014 Charles University in Prague
 * Copyright (c) 2014 Vojtech Horky
 * Copyright (c) 2018 Christoph Reichenbach
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
#include <stdio.h>
#include <papi.h>

#define JAVA_HEADER "/*\n" \
	" * This is generated file. Do not edit.\n" \
	" * To change this file, see the jni/genconst.c script and run\n" \
	" * make afterwards.\n" \
	" */\n\n" \
	"package papi;\n" \
	"\n" \
	"public class Constants {\n" \
	"\tpublic static final java.util.Map<Integer, String> ERRORS = new java.util.HashMap<>();\n" \
	"\tpublic static final java.util.Map<String, Integer> EVENTS = new java.util.HashMap<>();\n"

#define JAVA_FOOTER "}"

#define START_GROUP(description) \
	printf("\n\t/* %s */\n", description)

#define PRINT_CONST(name) \
	printf("\tpublic static final int %s = %d;\n", #name, name);

#define PRINT_CONST2(name, _)						\
	printf("\tpublic static final int %s = %d;\n", #name, name);

#define PRINT_CONSTX(name) \
	printf("\tpublic static final int %s = 0x%x;\n", #name, name);

#define PRINT_ERROR_DESCRIPTION(name, description)					\
	printf("\t\tERRORS.put(%s, \"%s\");\n", #name, description);

#define PRINT_EVENT_MAPPING(name) \
	printf("\t\tEVENTS.put(\"%s\", %s);\n", #name, #name);

int main() {
	printf("%s\n", JAVA_HEADER);

	PRINT_CONSTX(PAPI_VER_CURRENT);

	START_GROUP("Error codes");
#define PRINT_ERROR PRINT_CONST2
#include "errors_list.c"
#undef PRINT_ERROR

	START_GROUP("Preset events");
#define PRINT_EVENT PRINT_CONSTX
#include "events_list.c"
#undef PRINT_EVENT

	START_GROUP("Error code descriptions");
	printf("\tstatic {\n");
#define PRINT_ERROR PRINT_ERROR_DESCRIPTION
#include "errors_list.c"
#undef PRINT_ERROR
	printf("\t}\n");

	START_GROUP("Event names map");
	printf("\tstatic {\n");
#define PRINT_EVENT PRINT_EVENT_MAPPING
#include "events_list.c"
#undef PRINT_ERROR
	printf("\t}\n");


	printf("%s\n", JAVA_FOOTER);

	return 0;
}
