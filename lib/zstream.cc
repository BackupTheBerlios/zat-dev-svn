// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <stdarg.h>
#include <stdio.h>
#include "zefile.h"
#include "zstream.h"
#include "zstring.h"

zstream::zstream()
{
	fd = NULL;
	clean = true;
	prune = false;
}

zstream::zstream(const char *fname, bool writable)
{
	fd = NULL;
	clean = true;
	prune = false;
	open(fname, writable);
}

bool zstream::open(const zstring &fname, bool writable)
{
	close();

	if ((fd = fopen(fname.c_str(), writable ? "w" : "r")) != NULL) {
		name = fname;
		prune = writable;
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
		if (clean && prune)
			remove(name.c_str());
		fd = NULL;
	}

	clean = true;
	prune = false;
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

	for (int c; (c = getc_unlocked(in)) != EOF; ) {
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

void zstream::write(const void *from, size_t sz)
{
	if (!is_open())
		throw zeclosedfilew();

	fwrite(from, 1, sz, reinterpret_cast<FILE *>(fd));
	clean = false;
}

void zstream::print(const char *format, ...)
{
	if (is_open()) {
		va_list vl;
		va_start (vl, format);
		vfprintf(reinterpret_cast<FILE *>(fd), format, vl);
		va_end(vl);
		clean = false;
	}
}

zstream& zstream::operator << (const zstring &str)
{
	write(str.c_str(), str.size() + 1);
	return *this;
}

zstream& zstream::operator << (size_t sz)
{
	zstring tmp;
	tmp.format("%u", sz);
	return (*this << tmp);
}
