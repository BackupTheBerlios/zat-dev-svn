// ZAPN, ZX Assembler Preprocessor and Normalizer.
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$

#include <stdio.h>

#include "zerror.h"
#include "zstate.h"

const char * zerror::c_str() const
{
	switch (code) {
	case ret_ok:
		return "all clear";
	case ret_bad_strip_options:
		return "invalid strip options";
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
		fprintf(stderr, "%s:%u: %s%s: %s\n", state.fname, state.line, prefix(), c_str(), arg);
	else
		fprintf(stderr, "%s:%u: %s%s\n", state.fname, state.line, prefix(), c_str());
}


void zerror::repex(const char *arg) const
{
	if (arg != NULL && *arg != '\0')
		fprintf(stderr, "%s%s: %s\n", prefix(), c_str(), arg);
	else
		fprintf(stderr, "%s%s\n", prefix(), c_str());
}
