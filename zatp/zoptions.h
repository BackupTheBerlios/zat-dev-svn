// ZAPN, ZX Assembler Preprocessor and Normalizer.
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$
//
// Command line options.

#ifndef __zapn_zoptions_h
#define __zapn_zoptions_h

#include <stdio.h>

class zoptions
{
public:
	// Output file stream.
	FILE *fout;
	// Strip options.
	struct strip {
		bool comments;
		bool inserts;
		bool lines;
		bool spaces;
	} strip;
public:
	// Initialization.
	zoptions();
	// Memory and file handle clean-up.
	~zoptions();
};

extern zoptions opt;

#endif // __zapn_zoptions_h
