// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <vector>

#include "zas.h"
#include "zinput.h"
#include "zcpu.h"
#include "zinst.h"
#include "zmemblk.h"
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
		if (opt.debug.filerd)
			debug("Looking for \"%s\" using absolute path.\n", meta.name.c_str());
		in.open(meta.name.c_str());
	} else {
		for (std::vector< zstring >::iterator it = cpu.incdir.begin(); it != cpu.incdir.end(); ++it) {
			zstring path = zstring().format("%s/%s", it->c_str(), meta.name.c_str());

			if (in.open(path.c_str())) {
				if (opt.debug.filerd)
					debug("Looking for \"%s\".. found.\n", path.c_str());
				break;
			} else if (opt.debug.filerd) {
				debug("Looking for \"%s\".. none.\n", path.c_str());
			}
		}
	}

	return in.is_open();
}

bool zinput::read(zstring &line)
{
	if (!in.read_uncommented(line))
		return false;
	meta.line++;
	return true;
}
