// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$
//
// Simple file based input-output class.  The main idea is to have
// a wrapper around fgets(), which the std::fstream class apparently
// lacks.

#ifndef __zat_zstream_h
#define __zat_zstream_h

#include "zstring.h"

class zstream
{
	// Opaque file hande (internally cast to FILE).
	void *fd;
	// Set when anything is written to the file.
	bool clean;
	// Set to remove() the file when it's closed.
	bool prune;
	// File name, stored primarily to remove() later.
	zstring name;
public:
	// Initializers.
	zstream();
	zstream(const char *fname, bool writable = false);
	~zstream() { close(); }
	// Statistics.
	bool is_open() const { return fd != 0; }
	bool is_eof();
	// Opens a file.
	bool open(const zstring &fname, bool writable = false);
	// Closes the file.
	void close();
	// Reads a line from the file, returns `false' upon EOF.
	bool read(zstring &str);
	// Raw output.
	void write(const void *from, size_t size);
	// Formatted output.
	void print(const char *format, ...);
	// Serialization.
	zstream& operator << (const zstring &);
	zstream& operator << (size_t);
	zstream& operator >> (zstring &);
	zstream& operator >> (size_t &);
	// Returns the value of the file position.
	size_t tell();
	// Moves file pointer.
	void seek(size_t);
};

#endif
