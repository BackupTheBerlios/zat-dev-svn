// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "zeusage.h"

zeoption::zeoption(const char *src, int c)
{
	asprintf(&msg, "%s: `%c'", src, c ? c : '?');
}

zeoption::~zeoption()
{
	free(msg);
}
