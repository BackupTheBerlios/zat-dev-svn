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
	// Optimizes the text, calculates the hint.
	void fixup();
public:
	// Hash values, stored to speed things up.
	size_t hinta;
	size_t hintv;
public:
	// Copies the text and calculates hash value.
	zinst(const zinst &src);
	zinst(const char *src) : text(src) { fixup(); }
	zinst(const zstring &src) : text(src) { fixup(); }
	// Statistics.
	const char *c_str() const { return text.c_str(); }
	const zstring & str() const { return text; }
	// Comparators.
	bool compa(const zinst &) const;
	bool compv(const zinst &) const;
	// Used by the map of atomic instructions.
	struct mapa
	{
		size_t operator()(const zinst &src) const
			{ return src.hinta; }
		bool operator()(const zinst &a, const zinst &b) const
			{ return (a.hinta == b.hinta) ? a.compa(b) : false; }
	};
	// Used by the map of variable instructions.
	struct mapv
	{
		size_t operator()(const zinst &src) const
			{ return src.hintv; }
		bool operator()(const zinst &a, const zinst &b) const
			{ return (a.hintv == b.hintv) ? a.compv(b) : false; }
	};
	// Extracts strings that correspond to parameters.
	bool get_args(const zstring &src, std::vector<zstring> *args) const;
};

#endif
