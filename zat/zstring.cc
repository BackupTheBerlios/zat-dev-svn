// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "zstring.h"

void zstring::capsize()
{
	for (std::string::iterator it = begin(); it != end(); ++it)
		*it = ::toupper(*it);
}

unsigned int zstring::hash() const
{
	unsigned int value = 0;

	for (std::string::const_iterator it = begin(); it != end(); ++it)
		value = value * 31 + static_cast<unsigned char>(*it);

	return value;
}

bool zstring::has_path() const
{
	for (std::string::const_iterator it = begin(); it != end(); ++it) {
#ifdef _WIN32
		if (*it == '\\')
			return true;
#endif
		if (*it == '/')
			return true;
	}

	return false;
}

zstring zstring::gettok(const char *&src, char sep)
{
	const char *org = src, *end;

	while (*src != '\0' && *src != sep)
		++src;

	end = src;

	if (*src == sep)
		++src;

	return zstring(org, end - org);
}

bool zstring::operator == (const char *src) const
{
	for (std::string::const_iterator it = begin(); it != end() && *src != 0; ++it, ++src)
		if (::toupper(*it) != ::toupper(*src))
			break;
	return (*src == 0);
}
