// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#pragma once
#include <string>

class zstring : public std::string
{
public:
	// Initializers.
	zstring() { }
	zstring(const char *src) : std::string(src) { }
	zstring(const char *src, size_t sz) : std::string(src, sz) { }
	// Capsize all characters.
	void capsize();
	// Calculates a hash value for the string.
	unsigned int hash() const;
	// Checks whether the string contains a path name.
	bool has_path() const;
	// Checks whether a character is a white space.
	static bool isws(char c) { return (c == ' ' || c == '\t'); }
};
