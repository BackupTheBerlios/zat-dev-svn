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
	"Copyright 2004-2005 " CONTACT ".\n"
	"This is free software; see the source for copying conditions. There is NO\n"
	"warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
	;

static void show_help()
{
	fprintf(stdout, "%s",
		"zat, the portable assembler, copyright (c) 2004-2005 " CONTACT "\n"
		"Usage: zat [options] sources...\n"
		"\n"
		"Options:\n"
		"  -c name         : change CPU type (defaults to Z80)\n"
		"  -d options      : configures debug output verbosity\n"
		"  -h              : this help screen; twice for more\n"
		"  -I path         : additional include directory\n"
		"  -m fname        : write segment map to this file\n"
		"  -M              : gain dependency information\n"
		"  -o filename     : change the output file name (defaults to zat.obj)\n"
		"  -q              : quiet, suppress unnecessary messages\n"
		"  -s filename     : dump symbols to the file\n"
		"  -S options      : configures symbol table format\n"
		"  -v              : display version number and exit\n"
		"  -W              : treat warnings as errors\n"
		"\n"
		"Debug options (mix them along with +/- signs):\n"
		"   a              : include all possible output\n"
		"   e              : show the symbol table being processed\n"
		"   l              : show line numbers (useful with other options only)\n"
		"   m              : show machine code for each instruction table item\n"
		"   r              : show files being searched across the path (see -I)\n"
		"   s              : show symbols as they are found\n"
		"   t              : show the instruction table being processed\n"
		"   T              : show timing information\n"
		"   0              : show instruction probes as they don't match\n"
		"   1              : show instruction probes as they do\n"
		"\n"
		"Symbol table options (same rules):\n"
		"   a              : include address for instructions that emit data\n"
		"   b              : include emitted machine code\n"
		"   f              : include output file names\n"
		"   l              : include labels\n"
		"   s              : include source code\n"
		"");
}

static void do_debug(const char *args)
{
	bool sign = true;

	while (*args != '\0') {
		switch (*args) {
		case '+':
			sign = true;
			break;
		case '-':
			sign = false;
			break;
		case 'a':
			opt.debug.symtab = sign;
			opt.debug.newsym = sign;
			opt.debug.instab = sign;
			opt.debug.filerd = sign;
			opt.debug.tplmatch = sign;
			opt.debug.mcode = sign;
			opt.debug.lines = sign;
			opt.debug.timing = sign;
			break;
		case 'e':
			opt.debug.symtab = sign;
			break;
		case 'l':
			opt.debug.lines = sign;
			break;
		case 'm':
			opt.debug.mcode = sign;
			break;
		case 'r':
			opt.debug.filerd = sign;
			break;
		case 's':
			opt.debug.newsym = sign;
			break;
		case 't':
			opt.debug.instab = sign;
			break;
		case 'T':
			opt.debug.timing = sign;
			break;
		case '0':
			opt.debug.tplmiss = sign;
			break;
		case '1':
			opt.debug.tplmatch = sign;
			break;
		default:
			throw zeoption("unknown debug option", *args);
		}

		++args;
	}
}

static void do_symbols(const char *args)
{
	bool sign = true;

	while (*args) {
		switch (*args) {
		case '+':
			sign = true;
			break;
		case '-':
			sign = false;
			break;
		case 'a':
			opt.symbols.addr = sign;
			break;
		case 'b':
			opt.symbols.code = sign;
			break;
		case 'f':
			opt.symbols.fnames = sign;
			break;
		case 'l':
			opt.symbols.labels = sign;
			break;
		case 's':
			opt.symbols.source = sign;
			break;
		default:
			throw zeoption("unknown symbol table option", *args);
		}

		++args;
	}
}

static void zmain(int argc, char * const * argv)
{
	zerror rc;

	for (char ch; (ch = getopt(argc, argv, "c:d:hI:m:Mo:qs:S:vW")) > 0; ) {
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
		case 'm':
			opt.map = optarg;
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
		case 'S':
			do_symbols(optarg);
			break;
		case 'v':
			fprintf(stdout, "%s\n", version);
			return;
		case 'W':
			opt.errstart = zsev_warning;
			break;
		default:
			throw zeusage();
		}
	}

	opt.open();
	cpu.init(opt.cpu);
	cpu.translate(argc - optind, argv + optind);
	cpu.resolve();

	if (opt.debug.timing) {
		debug("Table read in %u msec.\n", cpu.stat.tabtime);
		debug("Translation done in %u msec (%u lines processed).\n", cpu.stat.trantime, cpu.stat.lines);
		debug("Symbol table fixed in %u msec.\n", cpu.stat.fixtime);
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
