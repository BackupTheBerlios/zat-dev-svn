// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <stdio.h>
#include <stdlib.h>
#include "zefile.h"

zefile::zefile(const char *txt, const char *fname)
{
	asprintf(&msg, "%s: %s", txt, fname);
}

zefile::~zefile()
{
	free(msg);
}
