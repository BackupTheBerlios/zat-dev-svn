// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$
//
// Describes a virtual machine.

#ifndef __zas_zcpu_h
#define __zas_zcpu_h
#include <vector>
#include "zconst.h"
#include "zfoundry.h"
#include "zinput.h"
#include "zinst.h"
#include "zoutput.h"
#include "zymbol.h"

class zerror;
class zymbol;

class zcpu
{
	// The queue of input files.  Most recent last.
	std::vector<zinput> input;
	// The list of output files.
	std::vector<zoutput *> output;
	// Index within the output vector.
	unsigned int iout;
	// Instructions with fixed machine code (atomic).
	zfoundry mapa;
	// Instructions with parameters (variable).
	zfoundry mapv;
	// Symbols and expressions for delayed evaluation.
	std::vector<zymbol *> symbols;
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
	bool do_atomic(const zstring &i, zoutput &out);
	// Translates a variable command, returns `true' on success.
	// Resets the label if a .define command was executed.
	bool do_variable(const zstring &i, zoutput &out, zstring &label);
	// Evaluates the expression and emits the result.  If the expression
	// could not be evaluated, emits zeros and adds the expression to the
	// symbol table.
	void emit(const zstring &expr, opcode, zoutput &out, int base);
	// Returns time offset in msec.
	static size_t gettime(void);
public:
	// Search path for input files.
	std::vector< zstring > incdir;
	// Statistics.
	struct stat_s
	{
		// Instruction table parsing time.
		size_t tabtime;
		// Translation time.
		size_t trantime;
		// Symbol table fixup time.
		size_t fixtime;
		// Lines of code.
		size_t lines;
		// Initialization.
		stat_s()
		{
			lines = 0;
		}
	} stat;
public:
	// Initialization.
	zcpu();
	// Memory and file handle clean-up.
	~zcpu();
	// Initializes the CPU table.
	void init(const zstring &cpu_name);
	// Translate the specified input files.
	void translate(int argc, char * const *argv);
	// Resolves symbols.
	void resolve();
	// Misc.
	bool is_ready() const { return ready; }
};

extern zcpu cpu;

#endif
