// Zat Assembler Toolchain.
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$

#include "../configure.h"
#ifdef _DEBUG
#include <stdio.h>
#include <stdarg.h>
#include "zat.h"
#include "zoptions.h"

void debug(const char *format, ...)
{
	if (opt.debug) {
		va_list vl;
		va_start(vl, format);
		vfprintf(stderr, format, vl);
		va_end(vl);
	}
}

#endif
