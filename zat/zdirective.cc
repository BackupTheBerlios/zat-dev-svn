// ZAA, ZX Assembler assembler (umm).
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$

#include <ctype.h>

#include "zcpu.h"
#include "zdirective.h"
#include "zinst.h"
#include "zymbol.h"
#include "zoptions.h"
#include "zstring.h"

static zstring get_token(const char *&src)
{
	zstring rc;
	const char *orig;

	while (IsWS(*src))
		++src;

	orig = src;

	while (*src != ',' && *src != '\0')
		++src;

	// Trim trailing spaces.
	while (src != orig && IsWS(*(src - 1)))
		--src;

	rc = zstring(orig, src - orig);

	while (IsWS(*src))
		++src;

	if (*src == ',')
		++src;

	return rc;
}


zerror do_defw(const char *src)
{
	while (*src != '\0') {
		int value;
		zerror rc;
		zstring expr = get_token(src);

		switch (rc = zinst::evaluate(expr, value)) {
		case ret_ok:
			rc = cpu.emit_w(value);
			if (!rc.is_ok())
				return rc;
			break;
		case ret_undefined_label:
			zymbol::delay(expr.c_str(), cpu.addr, op_word);
			break;
		default:
			return rc;
		}
	}

	return ret_ok;
}


zerror do_defb(const char *src)
{
	while (*src != '\0') {
		int value;
		zerror rc;
		zstring expr = get_token(src);

		switch (rc = zinst::evaluate(expr, value)) {
		case ret_ok:
			rc = cpu.emit_b(value);
			if (!rc.is_ok())
				return rc;
			break;
		case ret_undefined_label:
			zymbol::delay(expr.c_str(), cpu.addr, op_byte);
			break;
		default:
			return rc;
		}
	}

	return ret_ok;
}


zerror do_org(const char *src)
{
	int value;
	zerror rc;

	switch (rc = zinst::evaluate(zstring(src), value)) {
	case ret_ok:
		cpu.addr = value;
		return ret_ok_nodata;
	default:
		return rc;
	}
}


zerror do_equ(const char *src)
{
	zerror rc;

	if (cpu.lastlabel == NULL)
		return ret_no_label;

	switch (rc = zinst::evaluate(zstring(src), cpu.lastlabel->get_value())) {
	case ret_ok:
		return ret_ok_nodata;
	default:
		return rc;
	}
}


zerror do_include(const char *)
{
	return ret_ok_nodata;
}


zdirective zdirs[] = {
	{ "EQU", do_equ },
	{ "ORG", do_org },
	{ "DEFB", do_defb },
	{ "DEFW", do_defw },
	{ "INCLUDE", do_include },
	{ NULL, NULL }
};
