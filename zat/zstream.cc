// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <stdarg.h>
#include <stdio.h>
#include "zat.h"
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

	if ((fd = fopen(fname, writable ? "w" : "r")) != NULL) {
		flockfile(reinterpret_cast<FILE *>(fd));
		return true;
	}

	return false;
}

void zstream::close()
{
	if (fd != NULL) {
		funlockfile(reinterpret_cast<FILE *>(fd));
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
	
	str.erase();

	if (is_eof())
		return false;

	// flockfile(in);

	int c;
	for (; (c = getc_unlocked(in)) != EOF; ) {
		if (c == '\r') {
			while (c == '\r')
				c = getc_unlocked(in);
			if (c != '\n' && c != EOF)
				ungetc(c, in);
			break;
		} else if (c == '\n')
			break;

		str.push_back(c);
	}

	// funlockfile(in);

	return true;
}

void zstream::print(const char *format, ...)
{
	if (is_open()) {
		va_list vl;
		va_start (vl, format);
		vfprintf(reinterpret_cast<FILE *>(fd), format, vl);
		va_end(vl);
	}
}
