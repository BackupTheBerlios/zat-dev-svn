// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "zymbol.h"

zlabel::zlabel(const char *lname, const char *ltext, zmemblk *block, size_t base) :
	zymbol(lname), expr(ltext)
{
	this->block = block;
	this->base = base;
}

zlabel::~zlabel()
{
}

bool zlabel::evaluate(list &)
{
	return false;
}
