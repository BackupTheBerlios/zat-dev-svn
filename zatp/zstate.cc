// ZAPN, ZX Assembler Preprocessor and Normalizer.
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$
//
// Current preprocessor state.

#include "zstate.h"

zstate state;

zstate::zstate()
{
	fname = NULL;
	line = 0;
}


zstate::~zstate()
{
}
