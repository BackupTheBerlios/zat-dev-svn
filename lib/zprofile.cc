// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <stdlib.h>
#include <vector>
#include "zerror.h"
#include "zstream.h"
#include "zstring.h"
#include "zprofile.h"

typedef bool (*zprofile_func)(int, char * const *);

static bool zprofile_line(const zstring &line, const char *prefix, zprofile_func func)
{
	std::vector<zstring> args;
	std::vector<const char *> opts;
	zstring::const_iterator it = line.begin(), beg;

	while (it != line.end() && *it != 0x00) {
		if (*it == 0x20 || *it == 0x09) {
			++it;
		} else if (*it == '\'' || *it == '\"') {
			char c = *it++;

			for (beg = it; it != line.end() && *it != c; ++it)
				;

			args.push_back(zstring(&*beg, it - beg));

			if (args.size() == 1 && args[0] != prefix)
				return true;

			if (it++ == line.end())
				throw zeprofile(line.c_str());
		} else {
			for (beg = it; it != line.end() && *it != 0x20 && *it != 0x09; ++it)
				;

			args.push_back(zstring(&*beg, it - beg));

			if (args.size() == 1 && args[0] != prefix)
				return true;
		}
	}

	if (args.size() < 2)
		return true;

	for (std::vector<zstring>::const_iterator it = args.begin() + 1; it != args.end(); ++it)
		opts.push_back(it->c_str());

	return func(opts.size(), const_cast<char * const *>(&opts[0]));
}

static bool zprofile_file(const char *fname, const char *prefix, zprofile_func func)
{
	zstream rc;

	if (!rc.open(fname))
		return true;

	for (zstring line; rc.read(line); ) {
		if (!zprofile_line(line, prefix, func))
			return false;
	}

	return true;
}

bool zprofile(const char *prefix, zprofile_func func)
{
	const char *home = getenv("HOME");

	if (home != NULL) {
		zstring path;
		path.format("%s/.zat.rc", home);
		if (!zprofile_file(path.c_str(), prefix, func))
			return false;
	}

	if (!zprofile_file("./.zat.rc", prefix, func))
		return false;

	return true;
}
