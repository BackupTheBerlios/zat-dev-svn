// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "zoutput.h"
#include "zmemblk.h"
#include "zoptions.h"

zoutput::zoutput(const char *fname) :
	name(fname)
{
	add();
	if (opt.fsym.is_open())
		opt.fsym.print("; output changed to \"%s\"\n", fname);
}

zoutput::~zoutput()
{
	for (std::vector<zmemblk *>::iterator it = blocks.begin(); it != blocks.end(); ++it) {
		delete *it;
	}
}

void zoutput::add()
{
	blocks.push_back(new zmemblk());
}
