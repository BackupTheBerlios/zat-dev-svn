// Zat Assembler Toolchain.
// Copyright (c) 2004 hex@mirkforce.net
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
zerror zinput::do_label(const char *&src, zoutput &out)
{
	zerror rc = ret_ok;

	if (isalpha(*src) || *src == '_') {
		zstring name;
		const char *tmp = src;

		while (!IsWS(*tmp) && *tmp != '\0')
			++tmp;

		name = zstring(src, tmp - src);

		rc = zymbol::install(name.c_str(), out.block());
	}

	while (IsWS(*src))
		++src;

	return rc;
}

zerror zinput::do_line(zoutput &out)
{
	zerror rc;
	char line[1024], *tmp;
	const char *str = line;
	vector < pair<int, string> > args;

	if (in == NULL) {
		return zerror(ret_inerr, "reading from a closed file");
	}

	if ((tmp = read_line(line, sizeof(line), in)) == NULL) {
		return ret_ok_nodata;
	}

	meta.line++;

	if ((rc = do_label(str, out)) >= ret_warnings)
		return rc;

	if ((rc = zinst::match(str, args)) == ret_ok) {
		// inst->render(out);
		return ret_ok;
	} else {
		return zerror(ret_syntax, str);
	}
}
