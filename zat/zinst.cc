// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "zat.h"
#include "zinst.h"

void zinst::fixup()
{
	bool got_space = false;
	zstring::const_iterator src;
	zstring::iterator dst = text.begin();

	for (src = text.begin(); src != text.end(); ) {
		if (*src == '"' || *src == '\'') {
			char c = *src;
			while (*src != c && src != text.end())
				*dst++ = *src++;
			if (src != text.end())
				*dst++ = *src++;
		} else if (zstring::isws(*src)) {
			if (!got_space) {
				*dst++ = ' ';
				got_space = true;
			}
			while (zstring::isws(*src))
				++src;
		} else {
			*dst++ = ::toupper(*src++);
		}
	}

	text.resize(dst - text.begin());

	hintv = 0;

	for (src = text.begin(); src != text.end() && *src != ' '; ++src)
		hintv = hintv * 5 + *src;

	hinta = hintv;

	while (src != text.end())
		hinta = hinta * 5 + *src++;
}

bool zinst::compa(const zinst &with) const
{
	return (text == with.text);
}

// With the gcc implementation at hand ("gcc version 3.3 20030304
// (Apple Computer, Inc. build 1671)") the current object is the
// map element being analyzed and `pattern' is the key for which
// the caller is looking up.
//
// This method assumes that both instructions have already been
// optimized, which means they contain uppercase characters (except
// for the quoted text), only one space and no comments at the
// end of the line.
bool zinst::compv(const zinst &pattern) const
{
	zstring::const_iterator src = pattern.text.begin();
	zstring::const_iterator dst = text.begin();

	while (src != pattern.text.end() && dst != text.end()) {
		if (*src == '@') {
			if (zstring::isquote(*src)) {
				char q = *src++;
				while (dst != text.end() && *dst != q)
					++dst;
				if (dst == text.end())
					return false;
				++dst;
			} else {
				while (*dst != ',' && dst != text.end())
					++dst;
			}

			// Now `dst' must point to the end of line or
			// a comma.  Skip one, if so.
			if (dst != text.end() && *dst == ',')
				++dst;
		} else if (*src++ != *dst++) {
			break;
		}
	}

	if (src == pattern.text.end() && dst == text.end()) {
		debug(" - match: '%s' vs. '%s'\n", pattern.text.c_str(), text.c_str());
		return true;
	} else {
		return false;
	}
}
