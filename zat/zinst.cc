// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "zat.h"
#include "zinst.h"

zinst::zinst(const zinst &src)
{
	text = src.text;
	hinta = src.hinta;
	hintv = src.hintv;
}

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
	debug(1, " --- comparing '%s' with '%s'\n", text.c_str(), with.text.c_str());
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
bool zinst::compv(const zinst &source) const
{
	char bracket = 0;
	zstring::const_iterator src = text.begin();
	zstring::const_iterator dst = source.text.begin();

	while (src != text.end() && dst != source.text.end()) {
		if (*src == '@') {
			++src;

			if (zstring::isquote(*dst)) {
				char q = *dst++;
				while (dst != source.text.end() && *dst != q)
					++dst;
				if (dst == source.text.end())
					return false;
				++dst;
			} else {
				while (*dst != ',' && (bracket != *dst) && dst != source.text.end())
					++dst;
			}

			// Now `dst' must point to the end of line or
			// a comma.  Skip one, if so.
			if (dst != source.text.end() && *dst == ',')
				++dst;
		} else {
			if (*src == '[')
				bracket = ']';
			else if (*src == '(')
				bracket = ')';
			else
				bracket = 0;

			if (*src++ != *dst++)
				break;
		}
	}

	if (src == text.end() && dst == source.text.end()) {
		debug(2, " - match: '%s' vs. '%s'\n", text.c_str(), source.text.c_str());
		return true;
	} else {
		debug(2, " - mismatch: '%s' vs. '%s'\n", text.c_str(), source.text.c_str());
		return false;
	}
}
