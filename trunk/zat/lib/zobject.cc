// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "zobject.h"

zobject::zobject()
{
}

zobject::~zobject()
{
}

bool zobject::open(const char *fname)
{
	if (!in.open(fname, false))
		return false;

	do {
		segment seg;
		if (!seg.read(in))
			return false;
		if (seg.blocks.size() != 0)
			segments.push_back(seg);
	} while (in.tell() < in.size());

	return true;
}

bool zobject::segment::read(zstream &in)
{
	size_t count;

	in >> name;
	in >> count;

	while (count-- != 0) {
		block b;
		b.read(in);
		if (b.size != 0)
			blocks.push_back(b);
	}

	return true;
}

void zobject::segment::block::read(zstream &in)
{
	in >> base;
	in >> size;
	offset = in.tell();
	in.seek(offset + size);
}
