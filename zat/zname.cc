// ZAA, ZX Assembler assembler (umm).
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$

#include <stdio.h>
#include <string.h>

#include "zname.h"

zname::zname()
{
	_hint = 0;
}


zname::zname(const char *src) :
	zstring(src)
{
	_hint = zstring::hint();
}


zname::zname(const char *src, size_t sz) :
	zstring(src, sz)
{
	_hint = zstring::hint();
}


zname::zname(const zname &src) :
	zstring(src)
{
	_hint = zstring::hint();
}


zname::~zname()
{
}


unsigned int zname::hint() const
{
	return _hint;
}


bool zname::operator == (const zname &src) const
{
	if (_hint != src._hint)
		return false;
	return (strcmp(c_str(), src.c_str()) == 0);
}


void zname::capsize()
{
	zstring::capsize();
	_hint = zstring::hint();
}
