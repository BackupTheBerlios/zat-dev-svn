// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "zerror.h"

class zusage : public zerror
{
public:
	zusage() { }
	~zusage() { }
	const char * c_str() { return "Invalid command line.  For help, type: zat -h\n";  }
	zseverity severity() const { return zsev_ok; }
};
