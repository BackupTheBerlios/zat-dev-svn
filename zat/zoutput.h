// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#pragma once
#include <string>
#include <vector>

class zmemblk;

class zoutput
{
	std::string name;
	std::vector<zmemblk *> blocks;
public:
	zoutput(const char *fname);
	~zoutput();
	zmemblk & block();
};
