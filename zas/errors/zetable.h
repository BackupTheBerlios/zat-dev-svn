// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#ifndef __zat_zetable_h
#define __zat_zetable_h

#include "zerror.h"
#include "../zinput.h"

class zetable : public zerror
{
	char *msg;
public:
	zetable(const char *text, const zinput &in);
	~zetable();
	const char * c_str() { return msg; }
};

#endif
