// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#ifndef __zaa_zoutput_h
#define __zaa_zoutput_h

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

#endif // __zaa_zoutput_h
