// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$
//
// Exceptions related to file operations.

#ifndef __zat_zefile_h
#define __zat_zefile_h
#include "zerror.h"

class zefile : public zerror
{
	char *msg;
public:
	zefile(const char *txt, const char *fname);
	~zefile();
	const char * c_str() { return msg; }
};

class zenofiles : public zerror
{
public:
	zenofiles() { }
	~zenofiles() { }
	const char * c_str() { return "no input files"; }
};

class zeclosedfile : public zerror
{
public:
	zeclosedfile() { }
	~zeclosedfile() { }
	const char * c_str() { return "reading from a closed file"; }
};

class zeclosedfilew : public zerror
{
public:
	zeclosedfilew() { }
	~zeclosedfilew() { }
	const char * c_str() { return "writing to a closed file"; }
};

class zerdeof : public zerror
{
public:
	zerdeof() { }
	~zerdeof() { }
	const char * c_str() { return "reading past end of file"; }
};

#endif
