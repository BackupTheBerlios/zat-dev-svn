// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#ifndef __zas_zas_h
#define __zas_zas_h

#if defined(HAVE_configure_h)
# include "../configure.h"
#endif

#include "zerror.h"

#ifndef dimof
# define dimof(arr) (sizeof(arr) / sizeof(arr[0]))
#endif

#if defined(_DEBUG)
void debug(const char *format, ...);
#else
inline void debug(const char *, ...) { }
#endif

#endif
