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
	host = 0;
	phase = 0;
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
	union {
		char c[2];
		short s;
	} u;

	u.s = s;

	emit(u.c[0]);
	emit(u.c[1]);
}
