// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$
//
// Describes a single input file.

#ifndef __zaa_zinput_h
#define __zaa_zinput_h

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
	// Extracts the label, if any.
	void do_label(const char *&src, zoutput &out);
public:
	// Sets the file name and initializes everything else.
	zinput(const char *fname);
	// Closes the file, deallocates buffers.
	~zinput();
	// Opens the current file.  Fails if the file could not
	// be opened for reading.
	bool open();
	// Processes one line and moves the pointer to the next
	// one.  Returns false if the file is over.
	bool do_line(zoutput &out);
	// Returns the name of the file.
	inline const char * name() const { return meta.name.c_str(); }
};

#endif // __zaa_zinput_h
