// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#ifndef __zat_zobject_h
#define __zat_zobject_h

#include <vector>
#include "zstream.h"
#include "zstring.h"

class zobject
{
	zstream in;
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
};

struct zobject::segment
{
	struct block;
	zstring name;
	std::vector<block> blocks;
	typedef std::vector<block>::const_iterator const_iterator;
	// Reads segment data from a file.
	bool read(zstream &in);
};

struct zobject::segment::block
{
	size_t base;
	size_t size;
	size_t offset;
	void read(zstream &in);
};

#endif
