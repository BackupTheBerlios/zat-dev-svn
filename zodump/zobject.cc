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

	while (true) {
		segment seg;

		if (!seg.read(in))
			return false;

		fprintf(stdout, "Segment read: %s\n", seg.name.c_str());
	}
}

bool zobject::segment::read(zstream &in)
{
	size_t count;

	in >> name;
	in >> count;

	while (count != 0) {
		block b;
		b.read(in);
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
