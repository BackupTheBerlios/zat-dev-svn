// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#ifndef __zat_zobject_h
#define __zat_zobject_h

#include <stdio.h>
#include <string>
#include <vector>

class zobject
{
	FILE *in;
public:
	// The list of segments.
	struct segment;
	std::vector<segment> segments;
	// Shortcut.
	typedef std::vector<segment>::const_iterator const_iterator;
	// Initializers.
	zobject();
	~zobject();
	// Reads a file.
	bool open(const char *fname);
	// Reads a null-terminated string from the file.
	bool read(std::string &to);
	static bool read(std::string &to, FILE *in);
};

struct zobject::segment
{
	struct block;
	std::string name;
	std::vector<block> blocks;
	typedef std::vector<block>::const_iterator const_iterator;
	// Reads segment data from a file.
	bool read(FILE *in);
};

struct zobject::segment::block
{
	size_t base;
	size_t size;
	size_t offset;
};

#endif
