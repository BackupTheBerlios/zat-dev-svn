// ZAA, ZX Assembler assembler (umm).
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$

#include <stdio.h>

#include "zcpu.h"
#include "zerror.h"

const char * zerror::c_str() const
{
	switch (code) {
	case ret_ok:
		return "all clear";
	case ret_ok_nodata:
		return "the command emitted nothing";
	case ret_syntax:
		return "syntax error";
	case ret_inerr:
		return "read error";
	case ret_outerr:
		return "write error";
	case ret_bad_cpu_table:
		return "bad instruction description table";
	case ret_bad_mnemo:
		return "bad mnemonic";
	case ret_bad_label:
		return "invalid label";
	case ret_bad_label_type:
		return "bad label type";
	case ret_dup_label:
		return "duplicate label";
	case ret_bad_constant:
		return "invalid constant";
	case ret_undefined_label:
		return "undefined label";
	case ret_bad_expression:
		return "bad expression syntax";
	case ret_undefined_symbols:
		return "one or more undefined symbols remain";
	case ret_no_label:
		return "the line does not contain a label (but it must)";
	case ret_byte_overflow:
		return "byte sized value overflow";
	case ret_word_overflow:
		return "word sized value overflow";
	default:
		return "unknown error";
	}
}


const char * zerror::prefix() const
{
	if (is_error())
		return "error: ";
	else if (is_warning())
		return "warning: ";
	else
		return "";
}


bool zerror::is_ok() const
{
	return (code < ret_warnings);
}


bool zerror::is_warning() const
{
	return (code < ret_errors);
}


bool zerror::is_error() const
{
	return (code >= ret_errors);
}


void zerror::repin(const char *arg) const
{
	if (arg != NULL && *arg != '\0')
		fprintf(stderr, "%s:%u: %s%s: %s\n", cpu.fname, cpu.line, prefix(), c_str(), arg);
	else
		fprintf(stderr, "%s:%u: %s%s\n", cpu.fname, cpu.line, prefix(), c_str());
}


void zerror::repex(const char *arg) const
{
	if (arg != NULL && *arg != '\0')
		fprintf(stderr, "%s%s: %s\n", prefix(), c_str(), arg);
	else
		fprintf(stderr, "%s%s\n", prefix(), c_str());
}
