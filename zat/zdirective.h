// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#ifndef __zaa_zdirectives_h
#define __zaa_zdirectives_h

#include "zat.h"

class zdirective
{
public:
	// The name of the directive.
	const char *name;
	// The handler function.
	zerror (* func)(const char *args);
};


// The array of directive definitions.
extern zdirective zdirs[];

#endif // __zaa_zdirectives_h
