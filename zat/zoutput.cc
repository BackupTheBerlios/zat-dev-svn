// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "zoutput.h"
#include "zmemblk.h"

zoutput::zoutput(const char *fname) :
	name(fname)
{
	blocks.push_back(new zmemblk());
}

zoutput::~zoutput()
{
	for (std::vector<zmemblk *>::iterator it = blocks.begin(); it != blocks.end(); ++it) {
		delete *it;
	}
}

zmemblk & zoutput::block()
{
	return * blocks[blocks.size() - 1];
}
