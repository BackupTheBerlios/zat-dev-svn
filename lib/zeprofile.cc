// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zerror.h"

zeprofile::zeprofile(const char *str)
{
	asprintf(&msg, "invalid profile string: %s", str);
}

zeprofile::~zeprofile()
{
	free(msg);
}
