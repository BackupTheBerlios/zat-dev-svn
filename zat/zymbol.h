// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$
//
// There are three types of symbols:
//
// 1. Ones emitting data.  This includes all expressions occuring inside
//    the parameter list of an instructions.  These are evaluated and
//    emitted as a sequence of bytes.

#pragma once
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
public:
	zymbol();
	zymbol(const char *text);
	virtual ~zymbol();
	// Evaluates the current expression using the list.  Returns
	// `true' on success, `false' if nothing could be done.
	virtual bool evaluate(list &all) = 0;
	// Misc.
	const char * c_str() const { return text.c_str(); }
	virtual const char * extra() const { return "unknown"; }
	bool isok() const { return ready; }
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
