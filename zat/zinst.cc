// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "zat.h"
#include "zinst.h"
#include "zcpu.h"

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
	return cpu.is_ready() ? get_args(source.text, NULL) : false;
}

bool zinst::get_args(const zstring &src, std::vector<zstring> *args) const
{
	int bracket = 0;
	zstring::const_iterator ti = text.begin(), tlim = text.end();
	zstring::const_iterator si = src.begin(), slim = src.end();

	if (args != NULL)
		args->clear();

	while (si != slim && ti != tlim) {
		if (*ti == '@') {
			zstring::const_iterator beg = si;

			// Double '@' matches everything.
			if (++ti != tlim && *ti == '@') {
				if (args != NULL)
					args->push_back(zstring(&*si, slim - si));
				return true;
			}

			if (zstring::isquote(*si)) {
				char q = *si++;
				while (si != slim && *si != q)
					++si;
				if (si == slim)
					return false;
				++si;
			} else {
				while (si != slim && *si != ',' && *si != bracket)
					++si;
			}

			if (args != NULL)
				args->push_back(zstring(&*beg, si - beg));
		} else {
			if (*ti == '[')
				bracket = ']';
			else if (*ti == '(')
				bracket = ')';
			else
				bracket = 0;

			if (*si++ != *ti++)
				break;
		}
	}

	if (si == slim && ti == tlim) {
		debug(9, " - match: this='%s' vs. src='%s'\n", text.c_str(), src.c_str());
		return true;
	} else {
		debug(9, " - mismatch: this='%s' vs. src='%s'\n", text.c_str(), src.c_str());
		return false;
	}
}
