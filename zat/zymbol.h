// ZAA, ZX Assembler assembler (umm).
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$
//
// Labels.  They can be of three types:
//
// 1. Labels with immediately known values (such as normal labels
//    or EQU with expressions that refer to known values).
//
//    These are easy.  We just add the label with the corresponding
//    value to the list and then just use it.
//
// 2. Expressions that emit machine code.
//
//    These are rather not complicated, either: if the expression
//    could be evaluated, we emit the code and forget about it;
//    if it refers to undefined labels, we emit zeroes and store
//    the expression for later resolution.  Delayed expressions
//    are normal labels which have a list of memory addresses and
//    data types they were used in.  When the expression successfully
//    resolves (on the second pass), we go through that list and
//    emit the code wherever we have skipped it.
//
// 3. Expressions that do not emit machine code but are used by
//    other expressions.
//
//    These are a true pain in the ass.  The problem is that we
//    cannot store a pointer to this expression (label -- remember
//    that expressions and labels are the same).  It may not emit
//    any code, it may just be an EQU referring to another label,
//    defined later.
//
//    We could just forbid such declarations and require all EQU
//    directives to use expressions that are already valid.  We
//    could, also, store the referred expression with a pointer
//    to the curernt one and delay its processing.  This could
//    be dangerous, but if we stop processing the list of delayed
//    expressions after the first unsuccessful iteration, it should
//    work.

#ifndef __zaa_zymbol_h
#define __zaa_zymbol_h

#include <vector>

#include "zname.h"
#include "zerror.h"

// Defines a single label.  Maintains two lists of labels:
// one for runtime use, one for later resolution.
class zymbol
{
	// A shorter type name for iteration.
	typedef std::vector<zymbol>::iterator iterator;
	// The list of all defined labels.
	static std::vector<zymbol> list;
	// The name of the label.
	zname name;
	// The value associated with the label.
	int value;
	// The list of addresses of byte arguments that refer to this
	// label.  On the first pass we do not store values of labels,
	// we fill the corresponding bytes with zero values and remember
	// where to go back later.
	std::vector<unsigned int> words;
	// The same as "words" but for single byte values.
	std::vector<unsigned int> bytes;
	// The same as "words" but for offset values.
	std::vector<unsigned int> offsets;
	// Installs a new label with the specified name and value.
	zymbol(const char *expr, int value);
	// Flushes all delayed machine code.  Must only be called
	// when the value had been resolved!
	void emit();
public:
	// Copies a label (used by vectors).
	zymbol(const zymbol &);
	// Memory clean-up.
	~zymbol();
	// Copies a label (used by vectors).
	zymbol& operator = (const zymbol &);
	// Used by std::sort().
	bool operator < (const zymbol &) const;
	// Returns true if the label has delayed emissions.  This is
	// primarily used before installing a label: if it has delayed
	// emissions, they ara all processed and the label becomes
	// normal; otherwise the label is reported as duplicate.
	bool is_delayed() const;
	// Returns true if the object is a label, false if an expression.
	bool is_label() const;
	// Returns the value of the label.
	const int& get_value() const;
	int& get_value();
	// Dumps all labels to the symbol file.
	static void dump();
	// Installs a new label or reports ret_duplicate_label.
	static zerror install(const char *expr, int value, bool delayed = false);
	// Finds a label or an expression, returns NULL if no luck.
	static zymbol* find(const char *);
	// Delays a label, remembers where to emit its value.
	static void delay(const char *expr, unsigned int pc, int mode);
	// Recalculates delayed expressions.  If anything is still unresolved,
	// returns an error, otherwise ret_ok.
	static zerror rescan();
};

#endif // __zaa_zymbol_h
