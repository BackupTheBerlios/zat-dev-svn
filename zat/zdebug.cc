// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "../configure.h"
#ifdef _DEBUG
#include <stdio.h>
#include <stdarg.h>
#include "zat.h"
#include "zoptions.h"

void debug(int level, const char *format, ...)
{
	if (opt.debug >= level) {
		va_list vl;
		va_start(vl, format);
		fprintf(stderr, "debug: ");
		vfprintf(stderr, format, vl);
		va_end(vl);
	}
}

#endif
