// Zat Assembler Toolchain.
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$

#ifndef __zaa_zoutput_h
#define __zaa_zoutput_h

#include <vector>

class zmemblk;

class zoutput
{
	std::vector<zmemblk *> blocks;
public:
	zoutput();
	~zoutput();
	zmemblk & block();
};

#endif // __zaa_zoutput_h
