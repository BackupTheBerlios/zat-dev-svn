// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <stdio.h>
#include <stdlib.h>
#include "zetable.h"

zetable::zetable(const char *text, const zinput &in)
{
	asprintf(&msg, "bad syntax table: %s [%s:%u]", text, in.name(), in.line());
}

zetable::~zetable()
{
	free(msg);
}
