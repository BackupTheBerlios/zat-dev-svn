// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "zymbol.h"

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

bool zexpression::evaluate(list &)
{
	return false;
}
