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

void zoutput::write(zstream &fobj) const
{
	if (opt.fmap.is_open())
		opt.fmap.print("Segment name: %s\n", name.c_str());

	fobj << name;
	fobj << blocks.size();

	for (std::vector<zmemblk *>::const_iterator it = blocks.begin(); it != blocks.end(); ++it)
		(*it)->write(fobj);
}
