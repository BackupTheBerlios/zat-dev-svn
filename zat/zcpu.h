// ZAA, ZX Assembler assembler (umm).
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$

#ifndef __zaa_zcpu_h
#define __zaa_zcpu_h

#include "support/timespec.h"

class zerror;

class zcpu
{
	class blockdef;
public:
	// current file name
	const char *fname;
	// current line number
	unsigned int line;
	// current translation address
	unsigned int addr;
	// Emulated memory size.
	unsigned int ramsize;
	// Emulated memory data.
	unsigned char *ramdata;
public:
	// The time when processing started.
	faeutil::timespec ts_begin;
	// The time when translation stopped.
	faeutil::timespec ts_translation;
	// The time when symbol processing stopped.
	faeutil::timespec ts_symbols;
public:
	// The label on the current line.
	class zymbol *lastlabel;
	// The address of the current line.
	unsigned int caddr;
	// The map of used bytes.  For listing only.
	unsigned int usemap;
public:
	// Initialization.
	zcpu();
	// Memory and file handle clean-up.
	~zcpu();
	// Emits a single byte at the current address.
	zerror emit_b(int);
	zerror emit_b(int, unsigned int at);
	zerror emit_w(int);
	zerror emit_w(int, unsigned int at);
	// Initializes temporary data.
	void initcom();
	// Marks last N bytes as unused.
	void unuse(unsigned int N);
};


class zcpu::blockdef
{
public:
	// Position at which the block starts (within the ram image)
	unsigned int at;
	// The size of the block, in bytes.
	unsigned int size;
public:
	blockdef();
};


extern zcpu cpu;

#endif // __zaa_zcpu_h
