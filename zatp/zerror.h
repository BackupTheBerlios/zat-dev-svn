// ZAPN, ZX Assembler Preprocessor and Normalizer.
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$

#ifndef __zapn_zerror_h
#define __zapn_zerror_h

// Available error codes.
typedef enum retc_e
{
	ret_ok,

	ret_warnings,

	ret_errors,
	ret_bad_strip_options,
	ret_read_error,
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

#endif // __zapn_zerror_h
