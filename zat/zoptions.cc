// Zat Assembler Toolchain.
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$

#include <stdlib.h>

#include "zerror.h"
#include "zoptions.h"

zoptions opt;

zoptions::zoptions()
{
	out = "zat.bin";
	cpu = "Z80";
	sym = NULL;
	fout = NULL;
	fsym = NULL;
	debug = 0;
	errstart = ret_errors;
	quiet = false;
}


zoptions::~zoptions()
{
	if (fsym != NULL)
		fclose(fsym);
	if (fout != NULL)
		fclose(fout);
}


bool zoptions::open()
{
	if (sym != NULL && (fsym = fopen(sym, "wb")) == NULL) {
		perror("could not open symbol file for writing");
		return false;
	}

	if (out != NULL && (fout = fopen(out, "wb")) == NULL) {
		perror("could not open output file for writing");
		return false;
	}

	return true;
}
