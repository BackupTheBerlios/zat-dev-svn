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

static const char *version =
	"zodump " VERSION "\n"
	"\n"
	"Copyright 2004-2005 " CONTACT ".\n"
	"This is free software; see the source for copying conditions. There is NO\n"
	"warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
	;
static void usage(int depth)
{
	fprintf(stdout, "%s",
		"zodump, zat object dumper, copyright (c) 2004-2005 " CONTACT "\n"
		"Usage: zodump [options] objects...\n"
		"\n"
		"Options:\n"
		"  -d              : show decimal addresses (normaly hexadecimal)\n"
		"  -h              : this help screen; twice for more\n"
		"  -v              : show version number and exit\n"
		"");

	if (depth < 2)
		return;

	fprintf(stdout, "%s",
		"\n"
		"This program shows the content of object files created by zat.\n"
		"Each file contains named code segments, each of which contains\n"
		"one or more binary blocks with known base address and space.\n"
		"Block names and addresses are specified in the source code and\n"
		"then used by utilities such as zobj2sna.\n"
		"");
}

static bool do_file(const char *fname, bool dec)
{
	zobject o;

	if (!o.open(fname)) {
		fprintf(stderr, "Could not read \"%s\" for reading.\n", fname);
		return false;
	}

	fprintf(stdout, "%s:\n", fname);

	for (zobject::const_iterator it = o.segments.begin(); it != o.segments.end(); ++it) {
		fprintf(stdout, "         S %s\n", it->name.c_str());

		for (zobject::segment::const_iterator cit = it->blocks.begin(); cit != it->blocks.end(); ++cit) {
			if (dec)
				fprintf(stdout, "%08u B %08u\n", cit->base, cit->size);
			else
				fprintf(stdout, "%08X B %08X\n", cit->base, cit->size);
		}
	}

	return true;
}

void zmain(int argc, char * const * argv)
{
	int help = 0;
	bool dec = false;

	for (char ch; (ch = getopt(argc, argv, "dhv")) > 0; ) {
		switch (ch) {
		case 'd':
			dec = true;
			break;
		case 'h':
			++help;
			break;
		case 'v':
			fprintf(stdout, "%s", version);
			return;
		default:
			throw zeusage();
		}
	}

	if (help != 0) {
		usage(help);
		return;
	}

	argc -= optind;
	argv += optind;

	if (argc == 0)
		throw zenofiles();

	while (argc != 0) {
		do_file(*argv, dec);
		--argc;
		++argv;
	}
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
