// Zat Assembler Toolchain.
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$

#include "zoutput.h"
#include "zmemblk.h"

zoutput::zoutput()
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
