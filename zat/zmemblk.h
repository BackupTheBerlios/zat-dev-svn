// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#ifndef __zaa_zmemblk_h
#define __zaa_zmemblk_h

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
		void *code;
		// The size of the code.
		unsigned int size;
		// The amount of allocated data (we use paged
		// allocations to optimize performance).
		unsigned int capacity;
		// Initializes the structure.
		data();
		// Deallocates the block.
		~data();
		// Expands the block by the given number of bytes.
		bool resize(unsigned int);
	} code;
public:
	// Initialization.
	zmemblk();
	~zmemblk();
	// Statistics.
	unsigned int size() const;
	void * data() const;
	// Code emission.  Fails with memory allocations.
	bool emit(char byte);
};

#endif // __zaa_zmemblk_h
