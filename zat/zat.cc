// Zat Assembler Toolchain.
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$

#include <getopt.h>
#include <stdio.h>

#include "zat.h"
#include "zcpu.h"
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

int main(int argc, char * const argv[])
{
	zerror rc = ret_ok;

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

	if ((rc = cpu.init(opt.cpu)) != ret_ok) {
		rc.report();
		return rc;
	}

	if ((rc = cpu.translate(argc - optind, argv + optind)) != ret_ok) {
		rc.report();
		return rc;
	}

	if ((rc = zymbol::rescan()) != ret_ok) {
		rc.report();
		return rc;
	}

	return rc;
}
