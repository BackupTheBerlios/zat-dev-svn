// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <ctype.h>
#include <stdlib.h>
#include <algorithm>

#include "zat.h"
#include "zcpu.h"
#include "zinst.h"
#include "zdirective.h"
#include "zymbol.h"
#include "zoptions.h"
#include "zstring.h"
#include "zesyntax.h"

vector<zinst> zinst::hash[123];

unsigned int zinst::maxlength = 8;

static inline int ctoh(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return 0;
}

/*
static bool is_word(const char *line, const char *word)
{
	while (*word != '\0') {
		if (toupper(*word) != toupper(*line))
			return false;
		++word, ++line;
	}

	return IsWS(*line) || (*line == '\0');
}

static bool is_hex(const char *src)
{
	bool fence = (*src == '#');

	if (fence)
		++src;

	while (isxdigit(*src))
		++src;

	if (!fence && toupper(*src++) != 'H')
		return false;

	return !(isalnum(*src) || *src == '_');
}

static bool is_dec(const char *src)
{
	while (isdigit(*src))
		++src;

	return !(isalnum(*src) || *src == '_');
}

static bool is_bin(const char *src)
{
	bool fence = (*src == '%');

	if (fence)
		++src;

	while (*src == '0' || *src == '1')
		++src;

	if (!fence && toupper(*src++) != 'B')
		return false;

	return !(isalnum(*src) || *src == '_');
}

zinst::zinst(const char *src, char *data)
{
	length = 0;
	name = strdup(src);
	atomic = strchr(src, '@') == NULL ? true : false;
	slot s = get_slot(name.c_str());

	for (iterator it = s.begin(); it != s.end(); ++it) {
		if (it->name == name) {
			delete this;
			return;
		}
	}

	for (char *state, *tok = zatok(data, ',', &state); tok != NULL; tok = zatok(NULL, ',', &state)) {
		int value = 0;

		if (strcmp(tok, "\\1") == 0) {
			value = op_byte;
			length += 1;
		} else if (strcmp(tok, "\\2") == 0) {
			value = op_word;
			length += 2;
		} else if (strcmp(tok, "\\3") == 0) {
			value = op_offset;
			length += 1;
		} else if (tok[0] != '\0' && tok[1] != '\0') {
			value = (ctoh(*tok++) << 4) | ctoh(*tok);
			length += 1;
		}

		codes.push_back(value);
	}

	s.push_back(*this);
}
*/

zinst::zinst()
{
	atomic = false;
}

zinst::~zinst()
{
}

zinst& zinst::operator = (const zinst &src)
{
	length = src.length;
	name = src.name;
	codes = src.codes;
	atomic = src.atomic;
	return *this;
}

bool zinst::operator < (const zinst &src) const
{
	return (strcmp(name.c_str(), src.name.c_str()) < 0);
}

void zinst::feed(const char *src)
{
	while (IsWS(*src))
		++src;
}

void zinst::optimize()
{
	for (unsigned int idx = 0; idx < dimof(hash); ++idx) {
		slot s = hash[idx];

		sort(s.begin(), s.end());

		for (iterator it = s.begin(); it != s.end(); ++it)
			if (it->length > maxlength)
				maxlength = it->length;
	}
}

// FIXME: implement it, actually.
zinst::slot& zinst::find(const char *)
{
	return hash[0];
}

// Finds an instruction that matches the source line, copies
// its arguments to the args vector, return `ret_ok' on success.
void zinst::match(const char *src, vector< pair<int, string> > &args)
{
	char line[1024];
	zname mnemo(src);
	slot &s = find(mnemo.c_str());

	normalize(line, sizeof(line), src);

	for (iterator it = s.begin(); it != s.end(); ++it) {
		if (*it == mnemo && !it->match1(line, args))
			return;
	}

	throw zesyntax(src);
}

// Checks whether the specified (normalized) source line matches the
// current instruction.
bool zinst::match1(const char *src, vector< pair<int, string> > &args)
{
	size_t idx = 0;

	args.clear();

	for (const char *m = name.c_str(); *m != 0; ++m) {
		if (*m == '@') {
			const char *tsrc = src;

			while (*src != ',' && *src != '\0') {
				if (*src == '"' || *src == '\'') {
					for (char c = *src++; *src != c && *src != '\0'; ++src);
					if (*src++ == 0)
						throw zesyntax(src, "mismatched quote");
				} else {
					++src;
				}
			}

			// insert fixed data
			while (idx < codes.size() && codes[idx] >= 0)
				args.push_back(pair<int, string>(codes[idx++], string()));

			// fail if there is no parameter needed (but we
			// were parsing a @, which is a place holder)
			if (idx >= codes.size())
				throw zesyntax(src, "too much parameters");

			// insert the parameters
			args.push_back(pair<int, string>(codes[idx++], string(tsrc, src - tsrc)));
		}

		else if (*m != *src)
			return false;
	}

	return true;
}

#ifdef NEVER
zerror zinst::find(const char *line, zinst *&cmd)
{
	slot s = get_slot(line);

	// skip leading spaces
	while (IsWS(*line))
		++line;

	if (*line == '\0')
		return ret_ok_nodata;

	// first scan for atomic instructions (with no args)
	for (iterator it = s.begin(); it != s.end(); ++it) {
		if (it->atomic && strcmp(it->name.c_str(), line) == 0) {
			cmd = &(*it);
			return ret_ok;
		}
	}

	// second scan for instructions with arguments
	for (iterator it = s.begin(); it != s.end(); ++it) {
		if (!it->atomic) {
			const char *src = it->name.c_str();
			const char *dst = line;

			while (*src != '\0') {
				if (*src == '@') {
					for (++src; *dst != '\0' && *dst != ')' && *dst != ']' && *dst != ','; ++dst);
				} else if (*src != *dst) {
					break;
				} else {
					++src, ++dst;
				}
			}

			if (*src == '\0' && *dst == '\0') {
				cmd = &(*it);
				return ret_ok;
			}
		}
	}

	return ret_bad_mnemo;
}
#endif

// Compacts an instruction close to the form that's used in the
// instruction set definition file.  Capitalizes everything,
// collapses spaces.  Returns true on success, false if the
// line was too long.
bool zinst::normalize(char *dst, size_t size, const char *src)
{
	char last = 0;
	const char *odst = dst;

	if (size == 0)
		return false;

	while (IsWS(*src))
		++src;

	while (*src != '\0') {
		if (size == 0)
			return false;

		if (*src == ';')
			break;

		if (IsWS(*src)) {
			// emit a space
			if (isalnum(last)) {
				*dst++ = last = ' ';
				--size;
			}

			// skip all spaces and punctuation
			while (IsWS(*src) || *src == ',' || *src == ')' || *src == ']') {
				// remove the previously emitted space
				// if we find a punctuation character.
				if (last == ' ' && !IsWS(*src)) {
					--dst, ++size;
					last = *(dst - 1);
				}
				// insert all characters except for the spaces.
				if (size != 0 && !IsWS(*src)) {
					*dst++ = *src;
					--size;
				}
				++src;
			}

			if (size == 0)
				return false;
		} else {
			*dst++ = last = toupper(*src++);
			--size;
		}
	}

	while (dst != odst && IsWS(*(dst - 1)))
		--dst, ++size;

	if (size == 0)
		--dst;

	*dst = '\0';

	return true;
}


/*
zerror zinst::evaluate(zstring expr, int &value)
{
	value = 0;

	for (const char *tmp = expr.c_str(); *tmp != '\0'; ) {
		int tval = 0;
		int sign = 1;

		while (*tmp != '\0') {
			if (*tmp == '-')
				sign *= -1;
			else if (*tmp == '+')
				;
			else if (*tmp == ' ')
				;
			else
				break;
			++tmp;
		}

		if (is_hex(tmp)) {
			bool fence = (*tmp == '#');
			if (fence)
				++tmp;
			while (isxdigit(*tmp)) {
				tval = (tval * 16) + (isdigit(*tmp) ? (*tmp - '0') : (toupper(*tmp) - 'A' + 10));
				++tmp;
			}
			if (!fence)
				++tmp;
		}
		else if (is_dec(tmp)) {
			while (isdigit(*tmp)) {
				tval = (tval * 10) + *tmp - '0';
				++tmp;
			}
		}
		else if (is_bin(tmp)) {
			bool fence = (*tmp == '%');
			if (fence)
				++tmp;
			while (*tmp == '0' || *tmp == '1') {
				tval = (tval * 2) + (*tmp == '1' ? 1 : 0);
				++tmp;
			}
			if (!fence)
				++tmp;
		}
		else if (*tmp == '$' && !(isalnum(*(tmp + 1)) || *(tmp + 1) == '_')) {
			// tval = cpu.addr;
		}
		else if (isalpha(*tmp) || *tmp == '_') {
			zymbol *lab;
			zstring label;
			const char *orig = tmp;

			while (isalnum(*tmp) || *tmp == '_')
				++tmp;
			label = zstring(orig, tmp - orig);

			if ((lab = zymbol::find(label.c_str())) == NULL || lab->is_delayed()) {
				zerror rc = ret_undefined_label;
				if (opt.debug >= 2) {
					// rc.repin(label.c_str());
				}
				return rc;
			} else {
				value = lab->get_value();
			}
		}
		else {
			zerror rc = ret_bad_expression;
			if (opt.debug) {
				// rc.repin(expr.c_str());
			}
			return rc;
		}

		value += sign * tval;
	}

	return ret_ok;
}


zerror zinst::evaluate(const char *&atcmd, const char *&atsrc, int mode)
{
	zerror rc;
	zstring expr;
	const char *tmp;
	int value, skipsize = 0;

	while (*atcmd != '@' && *atcmd == *atsrc)
		++atcmd, ++atsrc;

	tmp = atsrc;

	while (*atsrc != ')' && *atsrc != '\0')
		++atsrc;

	expr = zstring(tmp, atsrc - tmp);

	switch ((rc = evaluate(expr, value))) {
	case ret_undefined_label:
		if (opt.debug >= 2) {
			// fprintf(stderr, "%s:%u: expression evaluation delayed: %s\n", cpu.fname, cpu.line, expr.c_str());
		}
		// zymbol::delay(expr.c_str(), cpu.addr, mode);
		break;
	case ret_ok:
		break;
	default:
		return rc;
	}

	switch (mode) {
	case op_byte:
		if (rc == ret_undefined_label)
			skipsize = 1;
		// rc = cpu.emit_b(value);
		break;
	case op_word:
		if (rc == ret_undefined_label)
			skipsize = 2;
		// rc = cpu.emit_w(value);
		break;
	case op_offset:
		if (rc == ret_undefined_label)
			skipsize = 1;
		// rc = cpu.emit_b((rc == ret_ok) ? value - cpu.caddr : 0);
		break;
	}

	if (rc == ret_undefined_label) {
		rc = ret_ok;
	}

	if (skipsize != 0)
		cpu.unuse(skipsize);

	return rc;
}
*/
