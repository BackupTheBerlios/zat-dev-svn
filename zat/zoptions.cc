// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "zefile.h"
#include "zoptions.h"

zoptions opt;

zoptions::zoptions()
{
	out = "zat.bin";
	cpu = "Z80";
	sym = 0;
	fout = 0;
	fsym = 0;
	errstart = zsev_error;
	quiet = false;
}


zoptions::~zoptions()
{
}


void zoptions::open()
{
	if (sym != 0 && !fsym.open(sym, true))
		throw zefile("could not open symbol file for writing", sym);

	if (out != 0 && !fout.open(out, true))
		throw zefile("could not open output file for writing", out);
}
