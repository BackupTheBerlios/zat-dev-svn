// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "zymbol.h"

zymbol::zymbol()
{
	value = 0;
	block = 0;
	base = 0;
	ready = false;
}

zymbol::zymbol(const char *lname) :
	text(lname)
{
	value = 0;
	block = 0;
	base = 0;
	ready = false;
}

zymbol::~zymbol()
{
}

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
