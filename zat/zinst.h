// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#ifndef __zaa_zinst_h
#define __zaa_zinst_h

#include <vector>
#include "zstring.h"

// The instruction comparator class.  Used as the hash map key.
class zinst
{
	// The template of the instruction.
	zstring text;
	// Hash value, stored to speed things up.
	unsigned int hash;
public:
	// Copies the text and calculates hash value.
	zinst(const char *src);
	// Compares two instructions.  If hash values match,
	// applies this->text as a wildcard pattern to the
	// given string and returns `true' if they still match.
	bool operator == (const zinst &src);
};

#endif // __zaa_zinst_h
