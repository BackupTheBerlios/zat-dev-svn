// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <stdlib.h>

#include "zat.h"
#include "zmemblk.h"

zmemblk::data::data()
{
	code = 0;
	size = 0;
	capacity = 0;
}

zmemblk::data::~data()
{
	if (code != NULL)
		free(code);
}

bool zmemblk::data::resize(size_t add)
{
	size_t nsize = size + add;

	if (nsize > capacity) {
		unsigned int ncap = capacity + 1024;
		void *ncode = realloc(code, ncap);

		if (ncode == NULL)
			return false;

		code = reinterpret_cast<char *>(ncode);
		capacity = ncap;
	}

	size = nsize;
	return true;
}

zmemblk::zmemblk()
{
	base = 0;
	basex = NULL;
}

zmemblk::~zmemblk()
{
}

void zmemblk::emit(char c)
{
	unsigned int offset = code.size;

	if (code.resize(sizeof(c)))
		code.code[offset] = c;
}

void zmemblk::emit(short s)
{
	emit(static_cast<char>(s & 255));
	emit(static_cast<char>((s >> 8) & 255));
}

void zmemblk::set_origin(int value)
{
	base = value;
	basex = NULL;
}

void zmemblk::set_origin(zorigin *value)
{
	base = 0;
	basex = value;
}
