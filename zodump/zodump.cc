// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <getopt.h>
#include <stdio.h>
#include <string>
#include "../configure.h"
#include "zobject.h"
#include "zefile.h"
#include "zeusage.h"

static int usage()
{
	fprintf(stdout, "%s",
		"zodump, zat object dumper, copyright (c) 2004-2005 " CONTACT "\n"
		"Usage: zodump [options] objects...\n"
		"\n"
		"Options:\n"
		"  -d              : show decimal addresses (normaly hexadecimal)\n"
		"");
	return 1;
}

static bool do_file(const char *fname)
{
	zobject o;

	if (!o.open(fname)) {
		fprintf(stderr, "Could not read \"%s\" for reading.\n", fname);
		return false;
	}

	fprintf(stdout, "File: %s\n", fname);

	for (zobject::const_iterator it = o.segments.begin(); it != o.segments.end(); ++it) {
		fprintf(stdout, "  Segment: %s\n", it->name.c_str());

		for (zobject::segment::const_iterator cit = it->blocks.begin(); cit != it->blocks.end(); ++cit) {
			fprintf(stdout, "    Block at %04Xh, size: %04Xh.\n", cit->base, cit->size);
		}
	}

	return true;
}

bool zmain(int argc, char * const * argv)
{
	bool dec = false;

	for (char ch; (ch = getopt(argc, argv, "dh")) > 0; ) {
		switch (ch) {
		case 'd':
			dec = true;
			break;
		case 'h':
			return usage();
		default:
			throw zeusage();
		}
	}

	argc -= optind;
	argv += optind;

	if (argc == 0)
		throw zenofiles();

	while (argc != 0) {
		if (!do_file(*argv))
			return false;
		--argc;
		++argv;
	}

	return true;
}

int main(int argc, char * const * argv)
{
	try {
		zmain(argc, argv);
		return 0;
	} catch (zerror &e) {
		fprintf(stderr, "zodump: %s\n", e.c_str());
	}
}
