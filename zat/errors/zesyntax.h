// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#pragma once
#include "zerror.h"

class zesyntax : public zerror
{
	char *msg;
public:
	zesyntax(const char *src, const char *kind = 0);
	~zesyntax();
	const char * c_str() { return msg; }
};
