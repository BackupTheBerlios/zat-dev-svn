// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "zobject.h"

zobject::zobject()
{
	in = NULL;
}

zobject::~zobject()
{
	if (in != NULL)
		fclose(in);
}

bool zobject::read(std::string &to)
{
	to.clear();

	if (feof(in))
		return false;

	for (int c; (c = getc_unlocked(in)) != EOF; ) {
		if (c == 0)
			return true;
		to.push_back(c);
	}

	return false;
}

bool zobject::open(const char *fname)
{
	if ((in = fopen(fname, "rb")) == NULL)
		return false;

	flockfile(in);

	while (true) {
		segment seg;

		if (!seg.read(in))
			return false;
	}
}

bool zobject::segment::read(FILE *in)
{
	if (!zobject::read(name, in))
		return false;

	return true;
}
