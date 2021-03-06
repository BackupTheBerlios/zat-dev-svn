// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <getopt.h>
#include <stdio.h>

#include "zas.h"
#include "zcpu.h"
#include "zeusage.h"
#include "zoptions.h"
#include "zprofile.h"

static const char *version =
	"zas " VERSION "\n"
	"\n"
	"Copyright 2004-2005 " CONTACT ".\n"
	"This is free software; see the source for copying conditions. There is NO\n"
	"warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
	;

static void show_help(bool more)
{
	fprintf(stdout, "%s",
		"zas, the portable assembler, copyright (c) 2004-2005 " CONTACT "\n"
		"Usage: zas [options] sources...\n"
		"\n"
		"Options:\n"
		"  -c name         : change CPU type (defaults to Z80)\n"
		"  -d options      : configures debug output verbosity:\n"
		"     +            : enable the following options (on by default)\n"
		"     -            : disable the following options\n"
		"     a            : include all possible output\n"
		"     e            : show the symbol table being processed\n"
		"     i            : show the instruction table being processed\n"
		"     l            : show line numbers (useful with other options only)\n"
		"     m            : show machine code for each instruction table item\n"
		"     M            : show some memory related operations\n"
		"     r            : show files being searched across the path (see -I)\n"
		"     s            : show symbols as they are found\n"
		"     t            : timestamp debug messages\n"
		"     T            : show timing information\n"
		"     0            : show instruction probes as they don't match\n"
		"     1            : show instruction probes as they do\n"
		"  -h              : this help screen; twice for more\n"
		"  -I path         : additional include directory\n"
		"  -m fname        : write segment map to this file\n"
		"  -M              : gain dependency information\n"
		"  -o filename     : change the output file name (defaults to zas.obj)\n"
		"  -q              : quiet, suppress unnecessary messages\n"
		"  -s filename     : dump symbols to the file\n"
		"  -S options      : configures symbol table format:\n"
		"     +            : enable the following options (on by defaul)\n"
		"     -            : disable the following options\n"
		"     a            : include address for instructions that emit data\n"
		"     b            : include emitted machine code\n"
		"     f            : include output file names\n"
		"     l            : include labels\n"
		"     s            : include source code\n"
		"  -v              : display version number and exit\n"
		"  -W              : treat warnings as errors\n"
		"");

	if (!more)
		return;

	fprintf(stdout, "%s",
		"\n"
		"Before the command line is processed, zas reads the file ~/.zat.rc\n"
		"line by line, processing each one that starts with word \"zas\" as\n"
		"if it was added to the command line.  Each line may contain options\n"
		"as well as file names; the latter will be added to the input list\n"
		"and then processed in a bulk.\n"
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
			opt.debug.time = sign;
			opt.debug.symtab = sign;
			opt.debug.newsym = sign;
			opt.debug.instab = sign;
			opt.debug.filerd = sign;
			opt.debug.tplmatch = sign;
			opt.debug.mcode = sign;
			opt.debug.lines = sign;
			opt.debug.timing = sign;
			opt.debug.memory = sign;
			break;
		case 'e':
			opt.debug.symtab = sign;
			break;
		case 'i':
			opt.debug.instab = sign;
			break;
		case 'l':
			opt.debug.lines = sign;
			break;
		case 'm':
			opt.debug.mcode = sign;
			break;
		case 'M':
			opt.debug.memory = sign;
			break;
		case 'r':
			opt.debug.filerd = sign;
			break;
		case 's':
			opt.debug.newsym = sign;
			break;
		case 't':
			opt.debug.time = sign;
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

// Returns `true' if it's ok to continue.
static bool do_cmdline(int argc, char * const * argv)
{
	zerror rc;
	size_t help = 0;

	optind = 0;

	for (char ch; (ch = getopt(argc, argv, "c:d:hI:m:Mo:qs:S:vW")) > 0; ) {
		switch (ch) {
		case 'c':
			opt.cpu = optarg;
			break;
		case 'd':
			do_debug(optarg);
			break;
		case 'h':
			help++;
			break;
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
			return false;
		case 'W':
			opt.errstart = zsev_warning;
			break;
		default:
			throw zeusage();
		}
	}

	if (help > 0) {
		show_help(help > 1);
		return false;
	}

	argc -= optind;
	argv += optind;

	while (argc) {
		cpu.append(*argv);
		--argc, ++argv;
	}

	return true;
}

static void zmain()
{
	opt.open();
	cpu.init(opt.cpu);
	cpu.translate();
	cpu.resolve();

	if (opt.debug.timing) {
		debug("Table read in %u msec.\n", cpu.stat.tabtime);
		debug("Translation done in %u msec (%u lines processed, %u lps).\n", cpu.stat.trantime, cpu.stat.lines, (unsigned int)((double)cpu.stat.lines / (double)cpu.stat.trantime * 1000));
		debug("Symbol table fixed in %u msec.\n", cpu.stat.fixtime);
	}
}

int main(int argc, char * const * argv)
{
	try {
		if (!zprofile("zas", do_cmdline))
			return 0;
		if (!do_cmdline(argc, argv))
			return 0;
		zmain();
		return 0;
	} catch (zerror &e) {
		fprintf(stderr, "zas: %s\n", e.c_str());
		return 1;
	}
}
