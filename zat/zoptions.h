// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#ifndef __zat_zoptions_h
#define __zat_zoptions_h
#include "zstream.h"

class zoptions
{
public:
	// File names.
	const char *out;
	const char *cpu;
	const char *sym;
	// File handles.
	zstream fsym;
	zstream fout;
	// Debug verbosity.
	int debug;
	// The point where errors start.  Defaults to ret_errors,
	// but may be changed to ret_warnings by a command line switch.
	int errstart;
	// Set to suppress extraneous output.
	bool quiet;
	// Set to show timing.
	bool timing;
public:
	// Initialization.
	zoptions();
	// Memory and file handle clean-up.
	~zoptions();
	// Opens all necessary file handles.
	void open();
};

extern zoptions opt;

#endif
