// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#ifndef __zas_zoptions_h
#define __zas_zoptions_h

#include "zstream.h"
#include "zstring.h"

class zoptions
{
public:
	// File names.
	zstring cpu;
	zstring sym;
	zstring out;
	zstring map;
	// File handles.
	zstream fsym;
	zstream fmap;
	// The point where errors start.  Defaults to ret_errors,
	// but may be changed to ret_warnings by a command line switch.
	int errstart;
	// Set to suppress extraneous output.
	bool quiet;
	// Debug options.
	struct debug_s
	{
		bool time; // timestamp messages
		bool symtab; // symbol table post-processing
		bool newsym; // symbols as they are added to the table
		bool instab; // instruction table processing
		bool filerd; // files being read
		bool tplmatch; // matching templates
		bool tplmiss; // non-matching templates
		bool mcode; // machine code for each new instruction
		bool lines; // show line numbers
		bool timing; // collect timing info
		bool memory; // some memory allocations
		debug_s()
		{
			time = false;
			symtab = false;
			newsym = false;
			instab = false;
			filerd = false;
			tplmatch = false;
			tplmiss = false;
			mcode = false;
			lines = false;
			timing = false;
			memory = false;
		}
	} debug;
	struct symbol_s
	{
		bool addr; // addresses
		bool code; // machine code
		bool source; // source code
		bool fnames; // output file names
		bool labels; // show labels and their addresses
		symbol_s()
		{
			addr = false;
			code = false;
			source = false;
			fnames = false;
			labels = true;
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
