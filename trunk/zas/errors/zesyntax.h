// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#ifndef __zat_zesyntax_h
#define __zat_zesyntax_h

#include "zerror.h"
#include "../zinput.h"

class zesyntax : public zerror
{
	char *msg;
public:
	zesyntax(const char *src, const char *kind, zinput *input = NULL);
	~zesyntax();
	const char * c_str() { return msg; }
};

#endif
