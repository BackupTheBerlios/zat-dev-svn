// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$
//
// Describes a single input file.

#ifndef __zat_zinput_h
#define __zat_zinput_h
#include "zat.h"
#include "zstring.h"
#include "zstream.h"

class zoutput;

class zinput
{
	// File statistics.
	struct meta_s
	{
		zstring name;
		unsigned int line;
		meta_s(const char *name);
	} meta;
	// File source.
	zstream in;
public:
	// Sets the file name and initializes everything else.
	zinput(const char *fname);
	// Closes the file, deallocates buffers.
	~zinput();
	// Opens the current file.  Fails if the file could not
	// be opened for reading.
	bool open();
	// Reads a line from the current file, returns `false'
	// when the file is over.
	bool read(zstring &);
	// Returns the name of the file.
	inline const char * name() const { return meta.name.c_str(); }
	// Returns current line number.
	inline unsigned int line() const { return meta.line; }
};

#endif
