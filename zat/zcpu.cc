// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <stdio.h>
#include <stdlib.h>

#include "zcpu.h"
#include "zinst.h"
#include "zoptions.h"
#include "zefile.h"
#include "zstream.h"
#include "zesyntax.h"

zcpu cpu;

zcpu::zcpu()
{
	ready = false;
}

zcpu::~zcpu()
{
}

void zcpu::add_instr(const char *src)
{
	zstring mnemo;
	const char *sep;
	vector<int> codes;
	bool atomic = true;

	while (zstring::isws(*src))
		++src;

	if (*src == ';')
		return;

	if (*src == '\0')
		return;

	if ((sep = strchr(src, '|')) == NULL)
		throw zesyntax(src, "malformed instruction table");

	mnemo = zstring(src, (sep++) - src);

	// Extract machine code.
	while (*sep != 0) {
		int code;
		zstring tok = zstring::gettok(sep, ',');

		if (* tok.c_str() == '.') {
			if (tok == ".byte")
				code = op_byte;
			else if (tok == ".word")
				code = op_word;
			else if (tok == ".boffset")
				code = op_boffset;
			else if (tok == ".zap")
				code = op_zap;
			else if (tok == ".define")
				code = op_define;
			else if (tok == ".origin")
				code = op_origin;
			else if (tok == ".include")
				code = op_include;
			else if (tok == ".insert")
				code = op_insert;
			else if (tok == ".bytelist")
				code = op_blist;
			else if (tok == ".wordlist")
				code = op_wlist;
			else if (tok == ".namespace")
				code = op_namespace;
			else
				throw zesyntax(src, "unknown translation directive");
			atomic = false;
		} else {
			char *unused;
			code = strtol(tok.c_str(), &unused, 16);
		}

		codes.push_back(code);
	}

	if (atomic) {
		mapa[zinst(mnemo)] = codes;
	} else {
		mapv[zinst(mnemo)] = codes;
	}
}

void zcpu::init(const char *cpu_name)
{
	zstream in;
	zstring fname;

	if (strchr(cpu_name, '/') == NULL) {
		fname.format(PREFIX "/" SHAREPATH "/cpu/%s", cpu_name);
	} else {
		fname = cpu_name;
	}

	if (!in.open(fname.c_str()))
		throw zefile("could not open instruction table for reading", fname.c_str());

	for (zstring line; in.read(line); add_instr(line.c_str()));
	ready = true;

	output.push_back(new zoutput(opt.out));
}

// Translates all specified input files.  If a file could not be
// opened or no input files were specified, returns false.
void zcpu::translate(int argc, char * const *argv)
{
	while (argc != 0) {
		input.push_back(zinput(*argv));
		if (!input[input.size() - 1].open())
			throw zefile("could not open file for reading", *argv);
		--argc, ++argv;
	}

	if (input.size() == 0)
		throw zenofiles();

	while (input.size() != 0) {
		zinput &i = input[input.size() - 1];
		debug(1, "translating \"%s\".\n", i.name());
		while (parse(i, *output[iout]));
		input.pop_back();
	}
}

bool zcpu::parse(zinput &in, zoutput &out)
{
	zstring label, line;

	if (!in.read(line))
		return false;

	if (get_label(label, line) && opt.fsym.is_open()) {
		opt.fsym.print(";       ;                         ; ; %s\n", label.c_str());
	}

	if (line.size() != 0) {
		zinst inst(line);
		size_t offset = out.size();

		if (!do_atomic(inst, out) && !do_variable(inst, out, label)) {
			throw zesyntax(line.c_str(), "unknown instruction");
		}

		// Dump the code.
		if (opt.fsym.is_open()) {
			bool pc_sent = false;
			size_t lim = out.size();

			do {
				opt.fsym.print("; %04Xh ; ", offset);

				for (size_t idx = 0; idx < 8; ++idx, ++offset) {
					if (offset >= lim)
						opt.fsym.print("   ");
					else
						opt.fsym.print("%02X ", static_cast<unsigned char>(out[offset]));
				}

				if (!pc_sent) {
					opt.fsym.print("; %s\n", inst.c_str());
					pc_sent = true;
				} else {
					opt.fsym.print(";\n");
				}
			} while (offset < lim);
		}
	}

	return true;
}

bool zcpu::get_label(zstring &label, zstring &line)
{
	zstring::const_iterator src = line.begin();

	while (src != line.end() && !zstring::isws(*src))
		++src;

	label = zstring(line.c_str(), src - line.begin());

	while (src != line.end() && zstring::isws(*src))
		++src;

	line.erase(0, src - line.begin());

	return label.size() != 0;
}

bool zcpu::do_atomic(zinst &inst, zoutput &out)
{
	mapa_t::const_iterator it = mapa.find(inst);

	if (it == mapa.end())
		return false;

	for (std::vector<int>::const_iterator bit = it->second.begin(); bit != it->second.end(); ++bit)
		out.emit(static_cast<char>(*bit));

	return true;
}

bool zcpu::do_variable(zinst &inst, zoutput &out, zstring &label)
{
	size_t base = out.size();
	mapv_t::const_iterator it = mapv.find(inst);

	if (it == mapv.end())
		return false;

	if (it->second.size() != 0) {
		std::vector<zstring> args;
		const std::vector<int> &codes = it->second;

		if (it->first.get_args(inst.str(), &args)) {
			if (opt.debug >= 4) {
				opt.fsym.print(";       the next instruction has %u parameters:\n", args.size());
				for (std::vector<zstring>::const_iterator it = args.begin(); it != args.end(); ++it) {
					opt.fsym.print(";       %u. %s\n", it - args.begin() + 1, it->c_str());
				}
			}
		}

		for (std::vector<int>::const_iterator it = codes.begin(); it != codes.end(); ++it) {
			if (*it >= 0) {
				out.emit(static_cast<char>(*it));
			} else {
				switch (*it) {
				case op_byte:
				case op_boffset:
					emit(args[0], op_byte, out, base);
					args.erase(args.begin());
					break;
				case op_word:
					emit(args[0], op_word, out, base);
					args.erase(args.begin());
					break;
				case op_zap:
					break;
				case op_define:
					label.clear();
					break;
				case op_origin:
					break;
				case op_include:
					break;
				case op_insert:
					break;
				case op_blist:
					break;
				case op_wlist:
					for (const char *src = args[0].c_str(); *src != 0; ) {
						zstring tok = zstring::gettok(src, ',');
						emit(tok, op_word, out, base);
					}
					args.erase(args.begin());
					break;
				default:
					throw zesyntax(inst.c_str(), "unsupported parameter");
				}
			}
		}
	}

	return true;
}

void zcpu::emit(const zstring &expr, opcode op, zoutput &out, size_t base)
{
	int value = 0;
	zstring::const_iterator it = expr.begin();

	if (!evaluate(it, value, base)) {
		value = 0;
		// pust a symbol
	}

	switch (op) {
	case op_byte:
		if (value < -128 || value > 255)
			throw zesyntax(expr.c_str(), "byte overflow");
		out.emit(static_cast<char>(value));
		break;
	case op_word:
		if (value < -32768 || value > 65535) {
			it = expr.begin();
			value = 0;
			evaluate(it, value, base);
			throw zesyntax(expr.c_str(), "word overflow");
		}
		out.emit(static_cast<short>(value));
		break;
	default:
		throw zesyntax(expr.c_str(), "unknown data type (internal error)");
	}
}

bool zcpu::evaluate(zstring::const_iterator &expr, int &value, size_t base)
{
	int tval = 0;
	char sign = '+';

	while (*expr != '\0') {
		if (*expr == '-' || *expr == '+' || *expr == '*' || *expr == '/') {
			sign = *expr++;
		}

		else if (sign == 0) {
			return false;
		}

		else if (*expr == '(') {
			++expr;

			if (!evaluate(expr, tval, base))
				return false;

			if (*expr != ')')
				return false;

			++expr;
		}

		else if (*expr == '#') {
			++expr;
			tval = get_hex(expr);
		}

		else if (*expr == '%') {
			++expr;
			tval = get_bin(expr);
		}

		else if (*expr == '$') {
			++expr;
			tval = base;
		}

		else if (isdigit(*expr)) {
			tval = get_dec(expr);
		}

		else {
			return false;
		}

		switch (sign) {
		case '+':
			value += tval;
			break;
		case '-':
			value -= tval;
			break;
		case '*':
			value *= tval;
			break;
		case '/':
			if (tval == 0)
				return false;
			value /= tval;
			break;
		}
	}

	return true;
}

int zcpu::get_hex(zstring::const_iterator &e)
{
	int value = 0;

	while (*e != 0) {
		if (*e >= '0' && *e <= '9')
			value = value * 16 + *e - '0';
		else if (*e >= 'a' && *e <= 'f')
			value = value * 16 + *e - 'a' + 10;
		else if (*e >= 'A' && *e <= 'F')
			value = value * 16 + *e - 'A' + 10;
		else {
			value = 0;
			break;
		}

		++e;
	}

	return value;
}

int zcpu::get_bin(zstring::const_iterator &e)
{
	int value = 0;

	while (*e != 0) {
		if (*e == '0')
			value = value * 2 + 0;
		else if (*e == '1')
			value = value * 2 + 1;
		else {
			value = 0;
			break;
		}
	}

	return value;
}

int zcpu::get_dec(zstring::const_iterator &e)
{
	int value = 0;
	zstring::const_iterator base = e;

	while (*e != 0) {
		if (*e >= '0' && *e <= '9')
			value = value * 10 + *e - '0';
		else if (*e == 'h' || *e == 'H') {
			e = base;
			value = get_hex(e);
			++e;
			break;
		} else {
			break;
		}
		++e;
	}

	return value;
}
