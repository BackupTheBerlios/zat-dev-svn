// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#ifndef __zat_zinst_h
#define __zat_zinst_h
#include <vector>
#include "zstring.h"

// The instruction comparator class.  Used as the hash map key.
class zinst
{
	// The template of the instruction.
	zstring text;
	// Hash value, calculated by the constructor.
	unsigned int hint;
	// Initializes the instruction.  Called by constructors.
	void init(const char *);
public:
	// Primary constructors.
	zinst(const char *src) { init(src); }
	zinst(const zstring &src) { init(src.c_str()); }
	// Copy constructor.
	zinst(const zinst &src);
	// Statistics.
	const char *c_str() const { return text.c_str(); }
	const zstring & str() const { return text; }
	unsigned int hash() const { return hint; }
	// Extracts strings that correspond to parameters.
	bool get_args(const zstring &src, std::vector<zstring> *args) const;
	// Optimizes the string (removes extra spaces, capitalizes,
	// etc).  This makes the process of matching straightforward.
	static void optimize(zstring &text);
	// Calculates hash value for an optimized string.
	static unsigned int calc_hash(const zstring &, bool atomic);
	// Compares with a line of source code.
	bool matchx(const zstring &) const;
	bool matcht(const zstring &) const;
};

#endif
