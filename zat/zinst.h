// ZAA, ZX Assembler assembler (umm).
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$

#ifndef __zaa_zinst_h
#define __zaa_zinst_h

#include <vector>

#include "zname.h"

class zerror;
class zstring;

// The zinst class defines a single assembler instruction read
// from the definition file.  The instruction consists of the
// normalized name and a set of machine codes that are generated
// in result.
//
// The list of instructions is stored in a static array, we use
// a simple hashing algorythm to make lookups a little faster.
class zinst
{
	// The list of chunks (hashing).
	static std::vector<zinst> hash[123];
	typedef std::vector<zinst>::iterator iterator;
	// A shorter typedef for a slot hash.
	typedef std::vector<zinst> & slot;
	// The normalized name of the command.
	zname name;
	// An array of generated machine code.  Special values
	// op_byte and op_word are used to refer to parameters
	// marked as @ and @@ respectively.
	std::vector<int> codes;
	// The length of the generated machine code, in bytes.
	// This is stored to simplify address caltulations,
	// because the list of codes may contain a single value
	// that produces two bytes (a reference to @@).
	unsigned int length;
	// The maximum length of a sequence of machine code.
	static unsigned int maxlength;
	// Set is the instruction has no arguments (neither @ nor @@).
	// During a lookup we first process "atomic" commands,
	// then process commands with parameters.  Primarily because
	// atomic commands are processed much faster (a single string
	// comparison).
	bool atomic:1;
	// Returns the slot where the command with the given normalized
	// name must be placed and looked for in.
	static slot get_slot(const char *);
private:
	// Normalizes the command: capitalizes all characters, removes
	// all extraneous spaces and so on.
	static bool normalize(char *dst, size_t size, const char *src);
	// Looks up an instruction by the normalized name.  If found,
	// returns ret_ok and "cmd" points to the instruction.  Note
	// that "cmd" may be null even if the function returns ret_ok.
	// This happens when CPU independent assembler directives such
	// as ORG, DEFB and so on are processed.
	static zerror find(const char *, zinst *&cmd);
	static bool find_dir(const char *line, zerror &rc);
	static bool find_asm(const char *line, zerror &rc);
public:
	// Evaluates an expression (performs label resolution and +,
	// -, * and / operations), fills value with the result.  If the
	// function fails, false is returned and the expression must be
	// evaluated later.
	static zerror evaluate(zstring expr, int &value);
	// Extracts the expression from the current translation position,
	// processes it and emits the machine code.  If the expression
	// could not be evaluated, it is stored for later resolution and
	// the function still returns ret_ok.
	static zerror evaluate(const char *&atcmd, const char *&atsrc, int mode);
public:
	// Initializes an instruction.  "Name" is the text of the
	// instruction, normalized.  "Data" is the generated machine
	// code, which may include \1 to refer to a byte and \2 to
	// refer to a word in the list of variable parameters of the
	// instruction.
	zinst(const char *name, char *data);
	// The copy constructor, used by vectors.
	zinst(const zinst &src);
	// Memory clean-up.
	~zinst();
	// Copies a command, used by vectors.
	zinst& operator = (const zinst &);
	// Compares two instructions, used when sorting the list
	// alphabetically.
	bool operator < (const zinst &) const;
	// Outputs the list of commands to the symbol file.
	static void dump();
	// Called after the list of instructions is processed and
	// final optimizations need to be done.  This includes
	// sorting the lists alphabetically.
	static void optimize();
	// Processes a line of source code.  Adds labels to the table,
	// finds the command and emits the data.  If an instruction
	// refers to a label that is not yet resolved, the function
	// stores the information into the list of unresolved labels
	// for later resolution.
	static zerror translate(const char *);
};


enum opcodes
{
	op_byte = -1,
	op_word = -2,
	op_offset = -3,
};


const char * get_opcode_name(int code);


#endif // __zaa_zinst_h
