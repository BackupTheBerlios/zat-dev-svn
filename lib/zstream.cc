// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../configure.h"
#include "zefile.h"
#include "zstream.h"
#include "zstring.h"

#define FD reinterpret_cast<FILE *>(fd)

#if !defined(HAVE_feof_unlocked)
# define feof_unlocked feof
#endif

#if defined(_WIN32)
static FILE* fixopen(const char *fname, const char *mode)
{
	FILE *rc;
	char *tempname = strdup(fname);

	for (char *tmp = tempname; *tmp != 0; ++tmp) {
		if (*tmp == '/')
			*tmp = '\\';
	}

	rc = fopen(tempname);
	free(tempname);
	return rc;
}
#else
# define fixopen fopen
#endif

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

	if ((fd = fixopen(fname.c_str(), writable ? "w" : "r")) != NULL) {
		name = fname;
		prune = writable;
		flockfile(FD);
		return true;
	}

	return false;
}

void zstream::close()
{
	if (fd != NULL) {
		funlockfile(FD);
		fclose(FD);
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
	return feof_unlocked(FD);
}

bool zstream::read(zstring &str)
{
	bool ok = false;

	str.erase();

	if (is_eof())
		return false;

	for (int c; (c = getc_unlocked(FD)) != EOF; ) {
		ok = true;

		if (c == '\r') {
			while (c == '\r')
				c = getc_unlocked(FD);
			if (c != '\n' && c != EOF)
				ungetc(c, FD);
			break;
		} else if (c == '\n')
			break;

		str.push_back(c);
	}

	return ok;
}

bool zstream::read_uncommented(zstring &str)
{
	if (!read(str))
		return false;

	for (zstring::const_iterator it = str.begin(); it != str.end(); ) {
		if (*it == ';') {
			for (zstring::const_iterator prev; it != str.begin(); it = prev) {
				prev = it;
				--prev;
				if (prev != str.begin() && *prev != 0x20 && *prev != 0x09)
					break;
			}

			str.resize(it - str.begin());
			return true;
		} else if (*it == '"' || *it == '\'') {
			char c = *it++;

			while (it != str.end() && *it != c)
				++it;

			// Fail to uncomment lines with mismatched quotes,
			// but don't fail to read them.
			if (it == str.end())
				return true;

			++it;
		} else {
			++it;
		}
	}

	return true;
}

void zstream::write(const void *from, size_t sz)
{
	if (!is_open())
		throw zeclosedfilew();

	fwrite(from, 1, sz, FD);
	clean = false;
}

void zstream::print(const char *format, ...)
{
	if (is_open()) {
		va_list vl;
		va_start (vl, format);
		vfprintf(FD, format, vl);
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

zstream& zstream::operator >> (zstring &to)
{
	if (!feof(FD)) {
		to.erase();

		for (int c; (c = getc_unlocked(FD)) != EOF; ) {
			if (c == 0)
				return *this;
			to.push_back(c);
		}
	}

	throw zerdeof();
}

zstream& zstream::operator >> (size_t &to)
{
	zstring tmp;
	*this >> tmp;
	to = static_cast<size_t>(atoi(tmp.c_str()));
	return *this;
}

size_t zstream::tell()
{
	long pos;
	if (fd == NULL)
		throw zeclosedfile();
	pos = ftell(FD);
	return pos >= 0 ? pos : 0;
}

void zstream::seek(size_t pos)
{
	if (fd == NULL)
		throw zeclosedfile();
	fseek(FD, pos, SEEK_SET);
}

size_t zstream::size()
{
	struct stat s;
	if (fstat(fileno(FD), &s))
		return 0;
	return s.st_size;
}
