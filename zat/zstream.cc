// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <stdio.h>
#include "zstream.h"
#include "zstring.h"

zstream::zstream()
{
	fd = NULL;
}

zstream::zstream(const char *fname, bool writable)
{
	fd = NULL;
	open(fname, writable);
}

bool zstream::open(const char *fname, bool writable)
{
	close();
	return (fd = fopen(fname, writable ? "w" : "rb")) != NULL;
}

void zstream::close()
{
	if (fd != NULL) {
		fclose(reinterpret_cast<FILE *>(fd));
		fd = NULL;
	}
}

bool zstream::is_eof()
{
	if (!is_open())
		return true;
	return feof(reinterpret_cast<FILE *>(fd));
}

bool zstream::read(zstring &str)
{
	FILE *in = reinterpret_cast<FILE *>(fd);
	
	str.clear();

	if (is_eof())
		return false;

	flockfile(in);

	for (int c; (c = getc_unlocked(in)) != EOF; ) {
		if (c == '\r' || c == '\n') {
			while (c == '\r' || c == '\n')
				c = getc_unlocked(in);
			if (c != EOF)
				ungetc(c, in);
			break;
		}

		str.push_back(c);
	}

	funlockfile(in);

	return true;
}

void zstream::print(const char *, ...)
{
}
