// ZAA, ZX Assembler assembler (umm).
// Copyright (c) 2004 hex@mirkforce.net
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
