// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#pragma once

class zmemblk
{
	// Origin host address.
	unsigned int host;
	// Origin phase address.
	unsigned int phase;
	// The machine code block.
	struct data
	{
		// The machine code.
		char *code;
		// The size of the code.
		size_t size;
		// The amount of allocated data (we use paged
		// allocations to optimize performance).
		size_t capacity;
		// Initializes the structure.
		data();
		// Deallocates the block.
		~data();
		// Expands the block by the given number of bytes.
		bool resize(size_t);
	} code;
public:
	// Initialization.
	zmemblk();
	~zmemblk();
	// Statistics.
	size_t size() const { return code.size; }
	const char * data() const { return code.code; }
	char & operator [] (size_t idx) { return code.code[idx]; }
	const char & operator [] (size_t idx) const { return code.code[idx]; }
	// Code emission.  Fails with memory allocations.
	void emit(char byte);
	void emit(short word);
};
