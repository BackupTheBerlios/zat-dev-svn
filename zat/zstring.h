// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#ifndef __zat_zstring_h
#define __zat_zstring_h
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
	// Compares with a standard string.
	bool operator == (const char *src) const;
	// Assignment.
	zstring& operator = (const zstring &src);
	// Formats a string.
	zstring& format(const char *format, ...);
	// Removes specified characters from the beginning and
	// from the end of the string.
	zstring& trim(char c = ' ');
	// Checks whether a character is a white space.
	static bool isws(char c) { return (c == ' ' || c == '\t'); }
	// Checks whether a character is a quotation mark.
	static bool isquote(char c) { return (c == '"' || c == '\''); }
	// Checks whether a character is an end of line.
	static bool iseol(char c) { return (c == '\r' || c == '\n'); }
	// Extracts a token from a string.
	static zstring gettok(const char *&src, char sep);
	// Adds the character to the hash.
	static void hadd(const_iterator &it, unsigned int &hval) { hval = hval * 5 + *it; }
};

#endif
