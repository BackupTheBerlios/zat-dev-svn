// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <stdio.h>
#include <stdlib.h>
#include "zetable.h"

zetable::zetable(const char *src, const char *kind)
{
	asprintf(&msg, "bad syntax table: %s: %s", kind, src);
}

zetable::~zetable()
{
	free(msg);
}
