// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#ifndef __zat_zprofile_h
#define __zat_zprofile_h

// Reads the content of ~/.zat.rc and dispatches all lines with
// the specified prefix as command lines to the given function.
// The user function returns true if it's ok to continue, so does
// this one.
bool zprofile(const char *prefix, bool (*func)(int, char * const *));

#endif
