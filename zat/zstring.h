// Zat Assembler Toolchain.
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$

#ifndef __zaa_zstring_h
#define __zaa_zstring_h

#include <string>

class zstring
{
	char *data;
public:
	zstring();
	zstring(const char *);
	zstring(const char *, size_t);
	zstring(const zstring &);
	~zstring();
	zstring& operator = (const zstring &);
	char * c_str();
	const char * c_str() const;
	void capsize();
	unsigned int hint() const;
	bool has_path() const;
};

#endif // __zaa_zstring_h
