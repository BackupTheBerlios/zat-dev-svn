// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$
//
// Exceptions related to symbols.

#pragma once
#include "zerror.h"

class zesymbol : public zerror
{
public:
	zesymbol() { }
	~zesymbol() { }
	const char * c_str() { return "duplicate label"; }
};
