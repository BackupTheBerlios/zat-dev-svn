// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "zoutput.h"
#include "zmemblk.h"
#include "zoptions.h"

zoutput::zoutput(const zstring &fname) :
	name(fname)
{
	add();

	if (opt.fsym.is_open() && opt.symbols.fnames)
		opt.fsym.print("; output changed to \"%s\"\n", fname.c_str());
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

void zoutput::show_map() const
{
	opt.fmap.print("  Segment name: %s\n", name.c_str());
	for (std::vector<zmemblk *>::const_iterator it = blocks.begin(); it != blocks.end(); ++it)
		(*it)->show_map();
}
