// Zat Assembler Toolchain.
// Copyright (c) 2004 hex@mirkforce.net
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

		code = ncode;
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

unsigned int zmemblk::size() const
{
	return code.size;
}

void * zmemblk::data() const
{
	return code.code;
}

bool zmemblk::emit(char c)
{
	unsigned int offset = code.size;

	if (!code.resize(sizeof(c)))
		return false;

	* (reinterpret_cast<char *>(code.code) + offset) = c;
	return true;
}