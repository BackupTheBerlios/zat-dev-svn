// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <getopt.h>
#include <stdio.h>

#include "zat.h"
#include "zcpu.h"
#include "zoptions.h"
#include "zeusage.h"

static const char *version =
	"zat " VERSION "\n"
	"\n"
	"Copyright 2004 hex@mirkforce.net.\n"
	"This is free software; see the source for copying conditions. There is NO\n"
	"warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
	;

static void show_help()
{
	fprintf(stdout, "%s",
		"zat, the portable assembler, copyright (c) 2004-2005 " CONTACT "\n"
		"Usage: zat [OPTIONS] sources...\n"
		"\n"
		"Options:\n"
		"  -c name         : change CPU type (defaults to Z80)\n"
		"  -d options      : include debug output, see -hh for details\n"
		"  -h              : this help screen; twice for more\n"
		"  -I path         : additional include directory\n"
		"  -M              : gain dependency information\n"
		"  -o filename     : default output file name\n"
		"  -q              : quiet, suppress unnecessary messages\n"
		"  -s filename     : dump symbols to the file\n"
		"  -t              : show process timing\n"
		"  -v              : display version number and exit\n"
		"  -W              : treat warnings as errors\n"
		"\n"
		"Debug options (mix all the necessary ones in a string):\n"
		"   a              : include all possible output\n"
		"   e              : show the symbol table being processed\n"
		"   r              : files being read\n"
		"   s              : show symbols as they are found\n"
		"   t              : show the instruction table being processed\n"
		"   T              : show instruction templates being matched\n"
		"");
}

static void do_debug(const char *args)
{
	while (*args != '\0') {
		switch (*args) {
		case 'a':
			opt.debug.symtab = true;
			opt.debug.newsym = true;
			opt.debug.instab = true;
			opt.debug.filerd = true;
			opt.debug.tplmatch = true;
			break;
		case 'e':
			opt.debug.symtab = true;
			break;
		case 'r':
			opt.debug.filerd = true;
			break;
		case 's':
			opt.debug.newsym = true;
			break;
		case 't':
			opt.debug.instab = true;
			break;
		case 'T':
			opt.debug.tplmatch = true;
			break;
		}

		++args;
	}
}

static void zmain(int argc, char * const * argv)
{
	zerror rc;

	for (char ch; (ch = getopt(argc, argv, "c:d:hI:Mo:qs:tvW")) > 0; ) {
		switch (ch) {
		case 'c':
			opt.cpu = optarg;
			break;
		case 'd':
			do_debug(optarg);
			break;
		case 'h':
			show_help();
			return;
		case 'I':
			cpu.incdir.push_back(optarg);
			break;
		case 'M':
			break;
		case 'o':
			opt.out = optarg;
			break;
		case 'q':
			opt.quiet = true;
			break;
		case 's':
			opt.sym = optarg;
			break;
		case 't':
			opt.timing = true;
			break;
		case 'v':
			fprintf(stdout, "%s\n", version);
			return;
		case 'W':
			opt.errstart = zsev_warning;
			break;
		default:
			throw zusage();
		}
	}

	opt.open();
	cpu.init(opt.cpu);
	cpu.translate(argc - optind, argv + optind);
	cpu.resolve();

	if (opt.timing) {
		fprintf(stdout, "Table read in %u msec.\n", cpu.stat.tabtime);
		fprintf(stdout, "Translation done in %u msec (%u lines processed).\n", cpu.stat.trantime, cpu.stat.lines);
		fprintf(stdout, "Symbol table fixed in %u msec.\n", cpu.stat.fixtime);
	}
}

int main(int argc, char * const * argv)
{
	try {
		zmain(argc, argv);
		return 0;
	} catch (zerror &e) {
		fprintf(stderr, "zat: %s\n", e.c_str());
		return 1;
	}
}
