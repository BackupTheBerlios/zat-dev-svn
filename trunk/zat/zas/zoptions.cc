// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "zefile.h"
#include "zoptions.h"

zoptions opt;

zoptions::zoptions()
{
	cpu = "Z80";
	out = "zas.obj";
	errstart = zsev_error;
	quiet = false;
}


zoptions::~zoptions()
{
}


void zoptions::open()
{
	if (sym.size() != 0 && !fsym.open(sym, true))
		throw zefile("could not open symbol file for writing", sym.c_str());

	if (map.size() != 0 && !fmap.open(map, true))
		throw zefile("could not open map file for writing", map.c_str());
}
