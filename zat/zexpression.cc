// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "zymbol.h"
#include "zmemblk.h"

zexpression::zexpression(const char *name, zmemblk *block, size_t base, size_t offset, enum opcode_e type) :
	zymbol(name)
{
	this->block = block;
	this->base = base;
	this->offset = offset;
	this->type = type;
}

zexpression::~zexpression()
{
}

bool zexpression::evaluate(list &symbols)
{
	int base;
	const char *e = text.c_str();

	if (isok())
		return true;

	value = 0;

	if (block && block->has_origin())
		base = block->get_origin() + this->base;
	else
		base = -1;

	if (zymbol::evaluate(e, value, base, symbols)) {
		ready = true;
		return true;
	}

	return false;
}
