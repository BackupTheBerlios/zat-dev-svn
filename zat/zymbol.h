// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#pragma once
#include <vector>
#include "zstring.h"

class zmemblk;

class zymbol
{
	// Text of the expression.
	zstring text;
	// Integer value to which the expression evaluates.
	int value;
	// Block to which the symbol applies, if it corresponds
	// to emitted data.
	zmemblk *block;
	// Position in the block.
	size_t offset;
	// Size of the emitted data.
	size_t size;
public:
	// Initializes an emitted synbol.
	zymbol(const char *text, zmemblk *block);
	// Initializes a constant.
	zymbol(const char *text);
	// Cleans up (what?)
	~zymbol() { }
	// Evaluates the symbol, returns `true' on success.
	bool eval(std::vector<zymbol> &all);
};
