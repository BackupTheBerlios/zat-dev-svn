// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <stdio.h>
#include <stdlib.h>
#include "zesyntax.h"

zesyntax::zesyntax(const char *src, const char *kind, zinput *input)
{
	if (input != NULL) {
		asprintf(&msg, "%s: %s [%s:%u]", kind, src, input->name(), input->line());
	} else {
		asprintf(&msg, "%s: %s", kind, src);
	}
}

zesyntax::~zesyntax()
{
	free(msg);
}
