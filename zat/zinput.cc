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

zinput::meta_s::meta_s(const char *fname) :
	name(fname)
{
	line = 0;
}

zinput::zinput(const char *fname) : meta(fname)
{
}

zinput::~zinput()
{
}

bool zinput::open()
{
	if (in.is_open())
		throw zefile("trying to open an already open file", name());

	if (meta.name.has_path()) {
		debug(1, "looking for \"%s\" using absolute path.\n", meta.name.c_str());
		in.open(meta.name.c_str());
	} else {
		for (std::vector< zstring >::iterator it = cpu.incdir.begin(); it != cpu.incdir.end(); ++it) {
			zstring path = zstring().format("%s/%s", it->c_str(), meta.name.c_str());

			if (in.open(path.c_str())) {
				break;
			}
		}
	}

	return in.is_open();
}

// Finds a label on the current line.  The pointer in the string
// is adjusted to the first non-space character after the label.
void zinput::do_label(const char *&src, zoutput &out)
{
	if (isalpha(*src) || *src == '_') {
		zstring name;
		const char *tmp = src;

		while (!zstring::isws(*tmp) && *tmp != '\0')
			++tmp;

		name = zstring(src, tmp - src);

		zymbol::install(name.c_str(), out.block());
	}

	while (zstring::isws(*src))
		++src;
}

bool zinput::do_line(zoutput & /* out */)
{
	zstring line;
	vector < pair<int, string> > args;

	if (!in.read(line))
		return false;

	meta.line++;

	// do_label(line.c_str(), out);
	// zinst::match(line.c_str(), args);

	return true;
}
