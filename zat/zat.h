// ZAA, ZX Assembler assembler (umm).
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$

#ifndef __zat_h_included__
#define __zat_h_included__

#if defined(HAVE_configure_h)
# include "../configure.h"
#endif

#include <stdio.h>

#include "zerror.h"
#include "zsupport.h"

#ifndef dimof
# define dimof(arr) (sizeof(arr) / sizeof(arr[0]))
#endif

FILE* zat_pre(int argc, const char **argv);

#endif // __zat_h_included__
