// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <stdlib.h>

#include "zat.h"
#include "zmemblk.h"

zmemblk::data::data()
{
	code = NULL;
	size = 0;
	capacity = 0;
}

zmemblk::data::~data()
{
	if (code != NULL)
		free(code);
}

bool zmemblk::data::resize(unsigned int add)
{
	size += add;

	if (add > capacity) {
		unsigned int ncap = capacity + 1024;
		void *ncode = realloc(code, ncap);

		if (ncode == NULL)
			return false;

		code = reinterpret_cast<char *>(ncode);
		capacity = ncap;
	}

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

bool zmemblk::emit(char c)
{
	unsigned int offset = code.size;

	if (!code.resize(sizeof(c)))
		return false;

	* (reinterpret_cast<char *>(code.code) + offset) = c;
	return true;
}
