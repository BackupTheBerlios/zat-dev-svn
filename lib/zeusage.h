// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#ifndef __zat_zeusage_h
#define __zat_zeusage_h
#include "zerror.h"

class zeusage : public zerror
{
public:
	zeusage() { }
	~zeusage() { }
	const char * c_str() { return "invalid command line; for help, use the -h switch";  }
	zseverity severity() const { return zsev_ok; }
};

#endif
