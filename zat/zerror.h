// ZAA, ZX Assembler assembler (umm).
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$

#ifndef __zaa_zerror_h
#define __zaa_zerror_h

// Available error codes.
typedef enum retc_e
{
	ret_ok,
	ret_ok_nodata,

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
};


// Error definition.
class zerror
{
	enum retc_e code;
	const char *prefix() const;
public:
	zerror(enum retc_e code = ret_ok) { this->code = code; }
	zerror(const zerror &src) { this->code = src.code; }
	operator const enum retc_e & () const { return code; }
	const char * c_str() const;
	int c_int() const { return this->code; }
	bool is_ok() const;
	bool is_warning() const;
	bool is_error() const;
	// Reports an error in the context of a file.
	void repin(const char *arg = 0) const;
	// Reports an error out of the context of a file.
	void repex(const char *arg = 0) const;
};

#endif // __zaa_zerror_h
