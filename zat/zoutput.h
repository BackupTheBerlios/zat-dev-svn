// Zat Assembler Toolchain.
// Copyright (c) 2004 hex@mirkforce.net
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
