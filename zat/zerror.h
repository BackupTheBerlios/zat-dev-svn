// Zat Assembler Toolchain.
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$

#ifndef __zaa_zerror_h
#define __zaa_zerror_h

#include <string>

// Available error codes.
typedef enum retc_e
{
	ret_ok,
	ret_ok_nodata,
	ret_no_input_files,

	ret_warnings,
	ret_byte_overflow = ret_warnings,
	ret_word_overflow,

	ret_errors,
	ret_syntax = ret_errors,
	ret_inerr,
	ret_outerr,
	ret_bad_cpu_table,
	ret_bad_mnemo,
	ret_bad_label,
	ret_bad_label_type,
	ret_dup_label,
	ret_bad_constant,
	ret_undefined_label,
	ret_bad_expression,
	ret_undefined_symbols,
	ret_no_label,
	ret_bracket_mismatch,
	ret_quote_mismatch,
	ret_too_much_parameters,
};


// Error definition.
class zerror
{
	enum retc_e code;
	std::string data;
	std::string temp;
	const char *prefix() const;
public:
	zerror(enum retc_e code = ret_ok, const char *msg = 0);
	zerror(const zerror &src);
	operator const enum retc_e & () const { return code; }
	const char * c_str();
	int c_int() const { return this->code; }
	bool is_ok() const;
	bool is_warning() const;
	bool is_error() const;
	// Reports a generic error.
	void report();
};

#endif // __zaa_zerror_h
