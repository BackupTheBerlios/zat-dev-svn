// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#ifndef __zname_h
#define __zname_h

#include "zstring.h"

class zname : public zstring
{
	unsigned int _hint;
public:
	zname();
	zname(const char *);
	zname(const char *, size_t);
	zname(const zname &);
	~zname();
	void capsize();
	unsigned int hint() const;
	bool operator == (const zname &) const;
};

#endif // __zname_h
