// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$
//
// Describes a virtual machine.

#ifndef __zaa_zcpu_h
#define __zaa_zcpu_h

#include <ext/hash_map>
#include <vector>
#include "zinput.h"
#include "zoutput.h"
#include "zinst.h"

class zerror;
class zymbol;

using std::vector;
using __gnu_cxx::hash_map;

class zcpu
{
	// The queue of input files.  Most recent last.
	vector<zinput> input;
	// The list of output files.
	vector<zoutput *> output;
	// Index within the output vector.
	unsigned int iout;
	// Instructions with fixed machine code.
	hash_map< int, int > fmap;
	// Adds a cpu instruction.
	void add_instr(const char *src);
public:
	// The label on the current line.
	zymbol *lastlabel;
	// The address of the current line.
	unsigned int caddr;
	// The map of used bytes.  For listing only.
	unsigned int usemap;
	// Search path for input files.
	vector< std::string > incdir;
public:
	// Initialization.
	zcpu();
	// Memory and file handle clean-up.
	~zcpu();
	// Initializes the CPU table.
	void init(const char *cpu_name);
	// Translate the specified input files.
	void translate(int argc, char * const *argv);
	// Initializes temporary data.
	void initcom();
	// Marks last N bytes as unused.
	void unuse(unsigned int N);
};


extern zcpu cpu;

#endif // __zaa_zcpu_h
