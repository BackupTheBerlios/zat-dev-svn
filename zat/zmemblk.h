// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#ifndef __zat_zmemblk_h
#define __zat_zmemblk_h
#include "zstring.h"

class zorigin;

class zmemblk
{
	// Base address.
	size_t base;
	// Set when the address is not known.
	zorigin *basex;
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
	// Changes the base address of the block.
	void set_origin(int);
	void set_origin(zorigin *);
	// Returns `true' if the origin is known.
	bool has_origin() const { return basex == NULL; }
	// Returns the base address.
	size_t get_origin() const { return (basex == NULL) ? base : 0; }
	// Prints segment map to the symbol file.
	void show_map() const;
};

#endif
