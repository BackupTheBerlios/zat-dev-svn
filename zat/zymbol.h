// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#ifndef __zat_zymbol_h
#define __zat_zymbol_h
#include <vector>
#include "zstring.h"
#include "zconst.h"

class zmemblk;

class zymbol
{
public:
	typedef const std::vector<zymbol *> list;
protected:
	// The name of the label or the expression text.
	zstring text;
	// Integer value of the expression.
	int value;
	// Memory block in which the symbol occured.
	zmemblk *block;
	// Base address of the command that has used the symbol.
	size_t base;
	// Set when the expression is successfully evaluated.
	bool ready;
	// Methods to convert a string from a specific base to a number.
	// Move the pointer to the character after the expression.
	static int get_hex(const char *&e);
	static int get_bin(const char *&e);
	static int get_dec(const char *&e);
	static bool get_symbol(const char *&e, int &value, list &);
public:
	zymbol();
	zymbol(const char *text);
	virtual ~zymbol();
	// Evaluates the current expression using the list.  Returns
	// `true' on success, `false' if nothing could be done.
	virtual bool evaluate(list &all) = 0;
	// Misc.
	const char * c_str() const { return text.c_str(); }
	virtual const char * extra() const { return NULL; }
	bool isok() const { return ready; }
	virtual bool islabel() const { return false; }
	// Returns the value.
	int get_value() const { return value; }
	// Evaluates the expression.  Upon success returns `true' and the
	// pointer is moved to the next character after the expression.
	// The `base' parameter holds the address of the currently
	// translated instruction.
	static bool evaluate(const char *&expr, int &value, int base, list &);
};

class zlabel : public zymbol
{
	// The expression that the label corresponds to.
	zstring expr;
public:
	zlabel(const char *name, const char *text, zmemblk *block, size_t base);
	~zlabel();
	bool evaluate(list &all);
	const char * extra() const { return expr.c_str(); }
	bool islabel() const { return true; }
};

class zexpression : public zymbol
{
	// Position in the code at which the data is emitted.
	size_t offset;
	// Type of the emitted data.
	enum opcode_e type;
public:
	zexpression(const char *name, zmemblk *block, size_t base, size_t offset, enum opcode_e type);
	~zexpression();
	bool evaluate(list &all);
};

class zorigin : public zymbol
{
public:
	zorigin(const char *expr, zmemblk *block);
	~zorigin();
	bool evaluate(list &all);
};

#endif
