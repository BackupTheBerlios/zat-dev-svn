// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "../configure.h"
#if defined(_DEBUG) || 1
#include <stdio.h>
#include <stdarg.h>
#include "zas.h"
#include "zcpu.h"
#include "zoptions.h"

void debug(const char *format, ...)
{
	va_list vl;
	va_start(vl, format);

	if (opt.debug.time) {
		size_t tm = zcpu::gettime();
		fprintf(stderr, "debug: %08u: ", tm);
	} else {
		fprintf(stderr, "debug: ");
	}

	vfprintf(stderr, format, vl);
	va_end(vl);
}

#endif
