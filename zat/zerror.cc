// Zat Assembler Toolchain.
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$

#include <stdio.h>
#include "zcpu.h"
#include "zerror.h"

zerror::zerror(enum retc_e code, const char *data)
{
	this->code = code;
	if (data != NULL)
		this->data = data;
}

zerror::zerror(const zerror &src)
{
	this->code = src.code;
	this->data = src.data;
}

const char * zerror::c_str()
{
	switch (code) {
	case ret_ok:
		temp = "all clear";
	case ret_ok_nodata:
		temp = "the command emitted nothing";
	case ret_no_input_files:
		temp = "no input files";
	case ret_syntax:
		temp = "syntax error";
	case ret_inerr:
		temp = "read error";
	case ret_outerr:
		temp = "write error";
	case ret_bad_cpu_table:
		temp = "bad instruction description table";
	case ret_bad_mnemo:
		temp = "bad mnemonic";
	case ret_bad_label:
		temp = "invalid label";
	case ret_bad_label_type:
		temp = "bad label type";
	case ret_dup_label:
		temp = "duplicate label";
	case ret_bad_constant:
		temp = "invalid constant";
	case ret_undefined_label:
		temp = "undefined label";
	case ret_bad_expression:
		temp = "bad expression syntax";
	case ret_undefined_symbols:
		temp = "one or more undefined symbols remain";
	case ret_no_label:
		temp = "the line does not contain a label (but it must)";
	case ret_byte_overflow:
		temp = "byte sized value overflow";
	case ret_word_overflow:
		temp = "word sized value overflow";
	default:
		temp = "unknown error";
	}

	if (data.length())
		temp += ": " + data;

	return temp.c_str();
}


const char * zerror::prefix() const
{
	if (is_error())
		return "error: ";
	else if (is_warning())
		return "warning: ";
	else
		return "zat: ";
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

void zerror::report()
{
	fprintf(stderr, "%s%s.\n", prefix(), c_str());
}
