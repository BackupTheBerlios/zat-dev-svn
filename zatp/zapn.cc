// ZAPN, ZX Assembler Preprocessor and Normalizer.
//
// $Id$
//
// The task of the preprocessor is to expand macros and enhanced commands
// to vanilla Z80 instruction set.  It also reformats the source code and
// adds comments with each command's execution time, where appropriate.

#include <stdio.h>
#include <getopt.h>
#include <strings.h>

#include "zapn.h"
#include "zerror.h"
#include "libfaeutil/fileio.h"

static zerror do_file(const char *fname);


static int usage()
{
	fprintf(stderr,
		"zapn, the zaa preprocessor and normalizer, copyright (c) 2004 " CONTACT "\n"
		"Usage: zapn [OPTIONS] sources...\n"
		"\n"
		"Options:\n"
		"  -D NAME      : mark symbols NAME as defined\n"
		"  -D NAME=VAL  : replace symbols NAME with value VAL\n"
		"  -I path      : add a directory to the search list\n"
		"  -l           : include file name and line information\n"
		"  -o filename  : output file, defaults to stdout\n"
		"  -s cils      : specifies the type of information to strip (see below)\n"
		"  -v           : print version number and exit\n"
		"\n"
		"Strip flags (the -s switch):\n"
		"   c           : remove comments\n"
		"   i           : replace inserts with series of DB\n"
		"   l           : do not add file names and line numbers\n"
		"   s           : collapse unnecessary spaces\n"
		);
	return 1;
}


static zerror do_strip(const char *flags)
{
	bool add = true;

	if (flags == NULL)
		return ret_bad_strip_options;

	while (*flags != '\0') {
		switch (*flags) {
		case '+':
			add = true;
			break;
		case '-':
			add = false;
			break;
		case 'c':
			opt.strip.comments = add;
			break;
		case 'i':
			opt.strip.inserts = add;
			break;
		case 'l':
			opt.strip.lines = add;
			break;
		case 's':
			opt.strip.spaces = add;
			break;
		default:
			return ret_bad_strip_options;
		}

		++flags;
	}

	return ret_ok;
}


static bool do_dir(const char *buf)
{
	char fname[1024];

	while (*buf != '\0') {
		if (strncasecmp(buf, "INCLUDE", 7) != 0) {
			buf++;
		} else if (buf[7] != ' ' && buf[7] != '\t') {
			buf++;
		} else {
			int c;
			char *tmp;

			buf += 8;

			while (*buf == ' ' || *buf == '\t')
				++buf;

			c = *buf++;

			if (c != '"' && c != '\'')
				return false;

			for (tmp = fname; *buf != '\0' && *buf != c; ++buf, ++tmp)
				*tmp = *buf;

			*tmp = '\0';

			do_file(fname);

			return true;
		}
	}

	return false;
}


/* reads and parses a file, calling itself recursively if needed */
static zerror do_file(const char *fname)
{
	char linebuf[1024];
	unsigned int line = 1;
	FILE *in = fopen(fname, "rb");

	if (in == NULL)
		return ret_read_error;

	while (freadline(linebuf, sizeof(linebuf), in) != NULL) {
		char *beg = linebuf, *cur = linebuf;

		while (*cur != '\0') {
			/* collapse spaces when asked to */
			if (opt.strip.spaces && (*cur == ' ' || *cur == '\t')) {
				while (*cur == ' ' || *cur == '\t')
					++cur;
				*beg++ = ' ';
			}

			/* comments always terminate processing */
			else if (*cur == ';') {
				if (opt.strip.comments) {
					while (beg != linebuf && (beg[-1] == ' ' || beg[-1] == '\t'))
						--beg;
				} else {
					while (*cur != '\0')
						*beg++ = *cur++;
				}
				break;
			}

			/* skip quoted strings (so that spaces wouldn't collapse) */
			else if (*cur == '"' || *cur == '\'') {
				int c = *cur;

				*beg++ = *cur++;

				while (*cur != c)
					*beg++ = *cur++;

				*beg++ = *cur++;
			}

			/* regular characters */
			else {
				*beg++ = *cur++;
			}
		}

		*beg = '\0';

		if (*linebuf == '\0' && opt.strip.spaces) {
			line++;
			continue;
		}

		if (do_dir(linebuf))
			continue;

		if (!opt.strip.lines)
			fprintf(opt.fout, "# %u %s\n", line, fname);

		fprintf(opt.fout, "%s\n", linebuf);

		line++;
	}

	return ret_ok;
}


int main(int argc, char * const argv[])
{
	zerror rc;
	const char *output = NULL;

	for (char ch; (ch = getopt(argc, argv, "DI:l:o:s:v")) != -1; ) {
		switch (ch) {
		case 'D':
			break;
		case 'I':
			break;
		case 'l':
			break;
		case 'o':
			if (output != NULL)
				return usage();
			output = optarg;
			break;
		case 's':
			if ((rc = do_strip(optarg)) != ret_ok) {
				rc.repex(optarg);
				return rc;
			}
			break;
		case 'v':
			fprintf(stdout, "zapn version %d.%d.%d.%d\n", VERSION);
			return 0;
		default:
			return usage();
		}
	}

	argc -= optind;
	argv += optind;

	if (argc == 0)
		return usage();

	if (output == NULL)
		opt.fout = stdout;
	else if ((opt.fout = fopen(output, "wb")) == NULL) {
		perror("Could not open output file");
		return 3;
	}

	while (argc != 0) {
		if ((rc = do_file(*argv)) != ret_ok) {
			rc.repex();
			return rc;
		}
		++argv;
		--argc;
	}

	fprintf(stdout, "OK.\n");
	return 0;
}
