// ZAPN, ZX Assembler Preprocessor and Normalizer.
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$
//
// Command line options.

#include "zoptions.h"

zoptions opt;

zoptions::zoptions()
{
	fout = NULL;

	strip.comments = false;
	strip.inserts = false;
	strip.lines = false;
	strip.spaces = true;
}


zoptions::~zoptions()
{
	if (fout != NULL)
		fclose(fout);
}
