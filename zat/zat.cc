// ZAA, ZX Assembler assembler (umm).
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$
//
// This program is the part of the toolkit that actually assembles the
// source code.  It does not know anything about a particular instruction
// set; it reads the necessary definition file and builds temporary lookup
// tables of CPU instructions and arguments.

#include <getopt.h>
#include <stdio.h>

#include "zat.h"
#include "zcpu.h"
#include "zinst.h"
#include "zymbol.h"
#include "zoptions.h"


static const char *version =
	"zat " VERSION "\n"
	"\n"
	"Copyright 2004 hex@mirkforce.net.\n"
	"This is free software; see the source for copying conditions. There is NO\n"
	"warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
	;

static int usage()
{
	fprintf(stdout, "%s",
		"zat: invalid command line.\n"
		"For help, type: zat -h\n"
		"");
	return ret_syntax;
}


static zerror setup_cpu()
{
	FILE *in;
	char buf[4096], *src;

	snprintf(buf, sizeof(buf), PREFIX "/" SHAREPATH "/cpu/%s", opt.cpu);

	if (opt.debug)
		fprintf(stdout, "reading instruction set from %s\n", buf);

	if ((in = fopen(buf, "rb")) == NULL)
		return ret_bad_cpu_table;

	while ((src = read_line(buf, sizeof(buf), in)) != NULL) {
		char *state;
		char *mnemo, *codes;

		while (IsWS(*src))
			++src;
		if (*src == ';')
			continue;

		strtoupper(src);

		mnemo = zatok(src, '|', &state);
		codes = zatok(NULL, '|', &state);

		if (codes == NULL)
			return ret_bad_cpu_table;

		new zinst(mnemo, codes);
	}

	zinst::optimize();
	return ret_ok;
}


static zerror do_file(const char *fname)
{
	char line[1024], *tmp;
	zerror rc = ret_ok;
	FILE *in = fopen(fname, "rb");

	if (in == NULL)
		return ret_inerr;

	cpu.fname = fname;
	cpu.line = 1;

	while ((tmp = read_line(line, sizeof(line), in)) != NULL) {
		const char *str = line;

		if ((rc = zinst::translate(str)) != ret_ok && rc != ret_ok_nodata)
			break;

		++cpu.line;
	}

	fclose(in);
	return rc;
}


int main(int argc, char * const argv[])
{
	zerror rc = ret_ok;
	std::vector<const char *> preargs;

	for (char ch; (ch = getopt(argc, argv, "c:dhI:Mo:qs:vW")) > 0; ) {
		switch (ch) {
		case 'c':
			opt.cpu = optarg;
			break;
		case 'd':
			opt.debug++;
			break;
		case 'h':
			fprintf(stdout, "%s",
				"zat, the portable assembler, copyright (c) 2004-2005 " CONTACT "\n"
				"Usage: zat [OPTIONS] sources...\n"
				"\n"
				"Options:\n"
				"  -c name         : change CPU type (defaults to Z80)\n"
#ifdef _DEBUG
				"  -d              : include debug output\n"
#endif 
				"  -h              : this help screen\n"
				"  -I path         : additional include directory\n"
				"  -M              : gain dependency information\n"
				"  -o filename     : default output file name\n"
				"  -q              : quiet, suppress unnecessary messages\n"
				"  -s filename     : dump symbols to the file\n"
				"  -v              : display version number and exit\n"
				"  -W              : treat warnings as errors\n"
				"");
			return ret_syntax;
		case 'I':
			preargs.push_back("-I");
			preargs.push_back(optarg);
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
		case 'v':
			fprintf(stdout, "%s\n", version);
			return 0;
		case 'W':
			opt.errstart = ret_warnings;
			break;
		default:
			return usage();
		}
	}

	argv += optind;
	argc -= optind;

	if ((rc = setup_cpu()) != ret_ok) {
		rc.repex();
		return rc;
	} else if (opt.debug) {
		zinst::dump();
	}

	if (!opt.open())
		return ret_outerr;

	cpu.ts_begin.update();

	while (argc != 0) {
		if ((rc = do_file(*argv)) != ret_ok) {
			rc.repin();
			if (rc.is_error())
				return rc;
		}
		++argv, --argc;
	}

	cpu.ts_translation.update();

	if ((rc = zymbol::rescan()) != ret_ok) {
		rc.repex();
		return rc;
	}

	cpu.ts_symbols.update();

	if (opt.fsym != NULL) {
		if (opt.debug)
			zinst::dump();
		zymbol::dump();
	}

	if (!opt.quiet)
		fprintf(stdout, "Translation time: %u msec.\nSymbol table fix-up: %u msec.\nTotal timing: %u msec.\n", (cpu.ts_translation - cpu.ts_begin).mseconds(), (cpu.ts_symbols - cpu.ts_translation).mseconds(), (cpu.ts_symbols - cpu.ts_begin).mseconds());

	return rc;
}
