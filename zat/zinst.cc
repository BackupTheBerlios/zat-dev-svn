// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "zat.h"
#include "zinst.h"
#include "zcpu.h"
#include "zoptions.h"

zinst::zinst(const zinst &src)
{
	text = src.text;
	hint = src.hint;
}

void zinst::init(const char *src)
{
	char br = 0;

	hint = 0;
	text = src;

	optimize(text);

	if (text.find('@') != zstring::npos)
		br = ' ';

	for (zstring::const_iterator it = text.begin(); it != text.end() && *it != br; ++it)
		zstring::hadd(it, hint);
}

void zinst::optimize(zstring &text)
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
}

unsigned int zinst::calc_hash(const zstring &src, bool atomic)
{
	unsigned int hval = 0;
	char br = atomic ? '\0' : ' ';

	for (zstring::const_iterator it = src.begin(); it != src.end() && *it != br; ++it)
		zstring::hadd(it, hval);

	return hval;
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

			if (*si != *ti)
				break;

			++si, ++ti;
		}
	}

	if (si == slim && ti == tlim) {
		if (opt.debug.tplmatch && args == NULL)
			debug("Template (+): \"%s\", source: \"%s\".\n", text.c_str(), src.c_str());
		return true;
	} else {
		if (opt.debug.tplmiss && args == NULL)
			debug("Template (-): \"%s\", source: \"%s\".\n", text.c_str(), src.c_str());
		return false;
	}
}

bool zinst::matchx(const zstring &src) const
{
	return (src == text);
}

bool zinst::matcht(const zstring &src) const
{
	return get_args(src, NULL);
}
