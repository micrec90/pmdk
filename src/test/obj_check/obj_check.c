/*
 * Copyright 2015-2018, Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     * Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * obj_check.c -- unit tests for pmemobj_check
 */

#include <stddef.h>

#include "unittest.h"
#include "libpmemobj.h"

int
main(int argc, char *argv[])
{
	START(argc, argv, "obj_check");
	if (argc < 2 || argc > 5)
		UT_FATAL("usage: obj_check <file> [-l <layout>] [-o]");

	const char *path = argv[1];
	const char *layout = NULL;
	PMEMobjpool *pop = NULL;
	int open = 0;

	for (int i = 2; i < argc; ++i) {
		if (strcmp(argv[i], "-o") == 0)
			open = 1;
		else if (strcmp(argv[i], "-l") == 0) {
			layout = argv[i + 1];
			i++;
		} else
			UT_FATAL("Unrecognized argument: %s", argv[i]);
	}

	if (open) {
		pop = pmemobj_open(path, layout);
		if (pop == NULL)
			UT_OUT("!%s: pmemobj_open", path);
		else
			UT_OUT("%s: pmemobj_open: Success", path);
	}

	int ret = pmemobj_check(path, layout);

	switch (ret) {
	case 1:
		UT_OUT("consistent");
		break;
	case 0:
		UT_OUT("not consistent: %s", pmemobj_errormsg());
		break;
	default:
		UT_OUT("error: %s", pmemobj_errormsg());
		break;
	}

	if (pop != NULL)
		pmemobj_close(pop);

	DONE(NULL);
}
