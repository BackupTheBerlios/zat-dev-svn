// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "zymbol.h"

zorigin::zorigin(const char *expr, zmemblk *block) :
	zymbol(expr)
{
	this->block = block;
}

zorigin::~zorigin()
{
}

bool zorigin::evaluate(list &)
{
	return false;
}
