// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <string.h>
#include <stdlib.h>
#include "zerror.h"

zemsg::zemsg(const char *msg)
{
	this->msg = strdup(msg);
}

zemsg::~zemsg()
{
	free(msg);
}

const char * zemsg::c_str()
{
	return msg;
}
