// Zat Assembler Toolchain.
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$
//
// A base for classes that emit code.

#ifndef __zaa_zemitter_h
#define __zaa_zemitter_h

class zerror;

class zemitter
{
public:
	// Emission.
	zerror emit_b(int);
	zerror emit_b(int, unsigned int at);
	zerror emit_w(int);
	zerror emit_w(int, unsigned int at);
};

#endif // __zaa_zemitter_h
