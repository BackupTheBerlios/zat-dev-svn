// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#ifndef __zas_zoutput_h
#define __zas_zoutput_h
#include <vector>
#include "zstring.h"
#include "zmemblk.h"

class zstream;

class zoutput
{
	zstring name;
	std::vector<zmemblk *> blocks;
public:
	zoutput(const zstring &fname);
	~zoutput();
	zmemblk & block() { return * blocks[blocks.size() - 1]; }
	const zmemblk & block() const { return * blocks[blocks.size() - 1]; }
	// Returns the size of the current block.
	size_t size() const { return block().size(); }
	// Returns a byte at the specified position.
	char & operator [](size_t idx) { return block()[idx]; }
	const char & operator[](size_t idx) const { return block()[idx]; }
	// Emits a byte.
	void emit(char c) { block().emit(c); }
	void emit(short s) { block().emit(s); }
	// Starts a new block.
	void add();
	// Writes the content to the object file.
	void write(zstream &fobj) const;
};

#endif
