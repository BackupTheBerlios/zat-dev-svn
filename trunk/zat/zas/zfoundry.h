// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$
//
// The zfoundry class stores instructions with corresponding machine
// code and provides quick lookups.

#ifndef __zas_zfoundry_h
#define __zas_zfoundry_h

#include <vector>
#include "zinst.h"

class zfoundry
{
public:
	typedef std::vector<int> data;
private:
	typedef std::pair <zinst, data > element;
	typedef std::vector< element > line;
	size_t size;
	line *lines;
	// Locates an instruction of a specific type.
	bool find(const zstring &, const data **, const zinst **, bool atomic);
public:
	zfoundry(size_t size);
	~zfoundry();
	// Inserts an element.
	void add(const zinst &, const data &);
	// Locates an instruction seeking an exact match.
	bool findx(const zstring &s, const data **d, const zinst **i = NULL) { return find(s, d, i, true); }
	// Locates an instruction applying the template.
	bool findt(const zstring &s, const data **d, const zinst **i = NULL) { return find(s, d, i, false); }
};

#endif
