// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$
//
// Exceptions related to symbols.

#ifndef __zat_zesymbol_h
#define __zat_zesymbol_h
#include "zerror.h"

class zesymbol : public zerror
{
public:
	zesymbol() { }
	~zesymbol() { }
	const char * c_str() { return "duplicate label"; }
};

#endif
