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
	// The point where errors start.  Defaults to ret_errors,
	// but may be changed to ret_warnings by a command line switch.
	int errstart;
	// Set to suppress extraneous output.
	bool quiet;
	// Debug options.
	struct debug_s
	{
		bool symtab; // symbol table post-processing
		bool newsym; // symbols as they are added to the table
		bool instab; // instruction table processing
		bool filerd; // files being read
		bool tplmatch; // matching templates
		bool tplmiss; // non-matching templates
		bool mcode; // machine code for each new instruction
		bool lines; // show line numbers
		bool timing; // collect timing info
		debug_s()
		{
			symtab = false;
			newsym = false;
			instab = false;
			filerd = false;
			tplmatch = false;
			tplmiss = false;
			mcode = false;
			lines = false;
			timing = false;
		}
	} debug;
	struct symbol_s
	{
		bool addr; // addresses
		bool code; // machine code
		bool source; // source code
		bool fnames; // output file names
		symbol_s()
		{
			addr = true;
			code = true;
			source = true;
			fnames = true;
		}
	} symbols;
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
