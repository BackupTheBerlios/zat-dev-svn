// ZAPN, ZX Assembler Preprocessor and Normalizer.
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$
//
// Current preprocessor state.

#ifndef __zapn_zstate_h
#define __zapn_zstate_h

#include <stdio.h>

class zstate
{
public:
	// Current file name.
	const char *fname;
	// Current line number.
	unsigned int line;
public:
	// Initialization.
	zstate();
	// Memory and file handle clean-up.
	~zstate();
};

extern zstate state;

#endif // __zapn_zstate_h
