// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$
//
// A base for classes that emit code.

#pragma once

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
