// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$
//
// Exceptions related to file operations.

#pragma once
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
