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

zinput::meta_s::meta_s(const char *fname)
{
	name = fname;
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
	typedef std::vector< std::string >::iterator sit;

	if (in != NULL)
		return true;

	for (sit it = cpu.incdir.begin(); it != cpu.incdir.end(); ++it) {
		std::string path = *it + "." + meta.name;

		if ((in = fopen(path.c_str(), "rb")) != NULL)
			break;
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
	zinst *inst;
	zerror rc = ret_ok;
	char line[1024], *tmp;
	const char *str = line;

	if ((tmp = read_line(line, sizeof(line), in)) == NULL)
		return ret_ok_nodata;

	meta.line++;

	if ((rc = do_label(str, out)) >= ret_warnings)
		return rc;

	if ((rc = zinst::find(str, inst)) == ret_ok) {
		// inst->render(out);
	}

	return ret_syntax;
}
