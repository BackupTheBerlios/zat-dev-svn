// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "zymbol.h"
#include "zmemblk.h"

zlabel::zlabel(const char *lname, const char *ltext, zmemblk *block, size_t base) :
	zymbol(lname), expr(ltext)
{
	this->block = block;
	this->base = base;
}

zlabel::~zlabel()
{
}

bool zlabel::evaluate(list &symbols)
{
	int base = -1;
	const char *e = expr.c_str();

	value = 0;

	if (block && block->has_origin())
		base = block->get_origin() + this->base;

	if (zymbol::evaluate(e, value, base, symbols)) {
		ready = true;
		return true;
	}

	return false;
}
