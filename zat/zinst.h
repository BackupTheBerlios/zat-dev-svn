// Zat Assembler Toolchain.
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$
//
// The zinst class contains information about a single assembly
// instruction, read from the external table and parsed for easier
// processing.  It also contains few methods to detect an instruction
// that corresponds to line of text.  The normal workflow is:
//
// - Call the zinst::feed() method, passing it a line of text read
//   from the external table; the text may contain any punctuation.
//   This is done as many times as there are lines in the external
//   table.
//
// - Call the zinst::optimize() method to sort the table and do
//   other optimization.
//
// - Call the zinst::match() method, passing it a single line of the
//   source code.  This method will fill the array of arguments with
//   substrings extracted from the original source.  If the line was
//   successfully matched, returne `ret_ok'.

#ifndef __zaa_zinst_h
#define __zaa_zinst_h

#include <vector>
#include <string>

#include "zname.h"

using namespace std;

class zerror;
class zstring;

class zinst
{
	// Hash table definition.
	typedef vector<zinst> slot;
	typedef slot::iterator iterator;
	static slot hash[123];
	// Returns the slot for an instruction.
	static slot& find(const char *);
	// peak length of ainstruction, in bytes.  Used for listing.
	static unsigned int maxlength;
	// The normalized name of the command.
	zname name;
	// An array of generated machine code.  Bytes of data are
	// positive, values from the `opcodes' enum are negative.
	vector<int> codes;
	// The length of the generated machine code, in bytes.
	// Stored to simplify calculations (one opcode may refer
	// to many bytes).
	unsigned int length;
	// Set if the instruction has no variable parameters.
	bool atomic;
	// Normalizes the command: capitalizes all characters,
	// removes all extraneous spaces and so on.
	static bool normalize(char *dst, size_t size, const char *src);
	// Matches the source against one command.
	zerror match1(const char *src, vector< pair<int, string> > &params);
public:
	// Finds a matching command in the table.  Fills the array
	// with paris of machine code and, optionally, the text of the
	// parameter.
	static zerror match(const char *src, vector< pair<int, string> > &params);
	// Installs a new command.
	static zerror feed(const char *src);
	// Optimizes the list.
	static void optimize();
	// Initialization.
	zinst();
	zinst(const zinst &s) { *this = s; }
	~zinst();
	// Copies a command, used by vectors.
	zinst& operator = (const zinst &);
	// Compares two instructions, used when sorting the list
	// alphabetically.
	bool operator < (const zinst &) const;
	// Compares the name of the instruction (name exluded).
	bool operator == (const zname &src) const { return name == src; }
};

enum opcodes
{
	op_byte = -1,
	op_word = -2,
	op_offset = -3,
	op_include = -4,
	op_origin = -5,
};

#endif // __zaa_zinst_h
