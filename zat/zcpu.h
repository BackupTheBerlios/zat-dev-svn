// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$
//
// Describes a virtual machine.

#pragma once
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
	// Instructions with fixed machine code (atomic).
	hash_map< zinst, vector<int>, zinst::mapa, zinst::mapa > mapa;
	// Instructions with parameters (variable).
	hash_map< zinst, vector<int>, zinst::mapv, zinst::mapv > mapv;
	// Adds a cpu instruction.
	void add_instr(const char *src);
	// Translates one line of the source code.  Returns `false'
	// when the file is over.
	bool parse(zinput &in);
	// Installs a label.
	bool get_label(zstring &label, zstring &line);
public:
	// The label on the current line.
	zymbol *lastlabel;
	// The address of the current line.
	unsigned int caddr;
	// The map of used bytes.  For listing only.
	unsigned int usemap;
	// Search path for input files.
	vector< zstring > incdir;
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

// Special values for machine code.
typedef enum opcode_e
{
	op_byte = -1,
	op_word = -2,
	op_boffset = -3,
	op_zap = -4,
	op_equ = -5,
} opcode;


extern zcpu cpu;
