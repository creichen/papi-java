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
#include <stdio.h>
#include <papi.h>

#define JAVA_HEADER "/*\n" \
	" * This is generated file. Do not edit.\n" \
	" * To change this file, see the jni/genconst.c script and run\n" \
	" * make afterwards.\n" \
	" */\n\n" \
	"package papi;\n" \
	"\n" \
	"class Constants {"

#define JAVA_FOOTER "}"

#define START_GROUP(description) \
	printf("\n\t/* %s */\n", description)

#define PRINT_CONST(name) \
	printf("\tpublic static final int %s = %d;\n", #name, name);

int main() {
	printf("%s\n", JAVA_HEADER);

	PRINT_CONST(PAPI_VER_CURRENT);

	START_GROUP("Error codes");
	PRINT_CONST(PAPI_OK);
	PRINT_CONST(PAPI_EINVAL);

	START_GROUP("Preset events");
	PRINT_CONST(PAPI_TOT_CYC);
	PRINT_CONST(PAPI_L1_DCM);


	printf("%s\n", JAVA_FOOTER);

	return 0;
}
