// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <vector>

#include "zat.h"
#include "zinput.h"
#include "zcpu.h"
#include "zinst.h"
#include "zmemblk.h"
#include "zymbol.h"
#include "zoptions.h"
#include "zefile.h"

using namespace std;

zinput::meta_s::meta_s(const char *fname)
{
	name = fname;
	line = 0;
}

zinput::zinput(const char *fname) : meta(fname)
{
	in = NULL;
}

zinput::~zinput()
{
	if (in != NULL)
		fclose(in);
}

bool zinput::open()
{
	if (in != NULL) {
		debug("file \"%s\" is already open.\n", name());
		return true;
	}

	if (meta.name.has_path()) {
		debug("looking for \"%s\" using absolute path.\n", meta.name.c_str());
		in = fopen(meta.name.c_str(), "rb");
	} else {
		for (std::vector< std::string >::iterator it = cpu.incdir.begin(); it != cpu.incdir.end(); ++it) {
			std::string path = *it + "/" + std::string(meta.name.c_str());

			debug("looking for \"%s\"...", path.c_str());
			if ((in = fopen(path.c_str(), "rb")) != NULL) {
				debug(" found!\n");
				break;
			} else {
				debug(" none.\n");
			}
		}
	}

	return (in != NULL);
}

// Finds a label on the current line.  The pointer in the string
// is adjusted to the first non-space character after the label.
void zinput::do_label(const char *&src, zoutput &out)
{
	if (isalpha(*src) || *src == '_') {
		zstring name;
		const char *tmp = src;

		while (!IsWS(*tmp) && *tmp != '\0')
			++tmp;

		name = zstring(src, tmp - src);

		zymbol::install(name.c_str(), out.block());
	}

	while (IsWS(*src))
		++src;
}

bool zinput::do_line(zoutput &out)
{
	char line[1024], *tmp;
	const char *str = line;
	vector < pair<int, string> > args;

	if (in == NULL)
		throw zeclosedfile();

	if ((tmp = read_line(line, sizeof(line), in)) == NULL)
		return false;

	meta.line++;

	do_label(str, out);
	zinst::match(str, args);

	return true;
}
