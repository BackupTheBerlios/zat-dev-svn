// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <stdio.h>
#include <stdlib.h>
#include "zesyntax.h"

zesyntax::zesyntax(const char *src, const char *kind)
{
	if (kind)
		asprintf(&msg, "%s: %s", kind, src);
	else
		asprintf(&msg, "%s", src);
}

zesyntax::~zesyntax()
{
	free(msg);
}
