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

// Special values for machine code.
typedef enum opcode_e
{
	op_byte = -1,
	op_word = -2,
	op_boffset = -3,
	op_zap = -4,
	op_define = -5,
	op_origin = -6,
	op_include = -7,
	op_insert = -8,
	op_blist = -9,
	op_wlist = -10,
	op_namespace = -11,
} opcode;

class zcpu
{
	typedef hash_map< zinst, vector<int>, zinst::mapa, zinst::mapa > mapa_t;
	typedef hash_map< zinst, vector<int>, zinst::mapv, zinst::mapv > mapv_t;
	// The queue of input files.  Most recent last.
	vector<zinput> input;
	// The list of output files.
	vector<zoutput *> output;
	// Index within the output vector.
	unsigned int iout;
	// Instructions with fixed machine code (atomic).
	mapa_t mapa;
	// Instructions with parameters (variable).
	mapv_t mapv;
	// Set when the instruction table is ready.  Used in zinst to
	// fail comparing one instruction with another.
	bool ready;
	// Adds a cpu instruction.
	void add_instr(const char *src);
	// Translates one line of the source code.  Returns `false'
	// when the file is over.
	bool parse(zinput &in, zoutput &out);
	// Installs a label.
	bool get_label(zstring &label, zstring &line);
	// Translates an atomic command, returns `true' on success.
	bool do_atomic(zinst &i, zoutput &out);
	// Translates a variable command, returns `true' on success.
	// Resets the label if a .define command was executed.
	bool do_variable(zinst &i, zoutput &out, zstring &label);
	// Evaluates the expression and emits the result.  If the expression
	// could not be evaluated, emits zeros and adds the expression to the
	// symbol table.
	void emit(const zstring &expr, opcode, zoutput &out, size_t base);
	// Evaluates the expression.  Upon success returns `true' and the
	// pointer is moved to the next character after the expression.
	bool evaluate(zstring::const_iterator &expr, int &value, size_t base);
	// Methods to convert a string from a specific base to a number.
	// Move the pointer to the character after the expression.
	static int get_hex(zstring::const_iterator &e);
	static int get_bin(zstring::const_iterator &e);
	static int get_dec(zstring::const_iterator &e);
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
	// Misc.
	bool is_ready() const { return ready; }
};

extern zcpu cpu;
