// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$
//
// Simple file based input-output class.  The main idea is to have
// a wrapper around fgets(), which the std::fstream class apparently
// lacks.

#pragma once
#include <string>

class zstream
{
	// Opaque file hande (internally cast to FILE).
	void *fd;
public:
	// Initializers.
	zstream();
	zstream(const char *fname, bool writable = false);
	~zstream() { close(); }
	// Statistics.
	bool is_open() const { return fd != NULL; }
	bool is_eof();
	// Opens a file.
	bool open(const char *fname, bool writable = false);
	// Closes the file.
	void close();
	// Reads a line from the file, returns `false' upon EOF.
	bool read(std::string &str);
	// Formatted output.
	void print(const char *format, ...);
};