// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <stdio.h>
#include <stdlib.h>
#ifndef _WIN32
# include <sys/time.h>
#else
# include <windows.h>
#endif
#include "zas.h"
#include "zcpu.h"
#include "zinst.h"
#include "zoptions.h"
#include "zefile.h"
#include "zstream.h"
#include "zetable.h"
#include "zesyntax.h"

using std::vector;

zcpu cpu;

zcpu::zcpu() : mapa(31), mapv(13)
{
	ready = false;
	incdir.push_back(".");
}

zcpu::~zcpu()
{
}

void zcpu::add_instr(const char *src, const zinput &in)
{
	zstring mnemo;
	const char *sep;
	vector<int> codes;
	bool atomic = true;

	while (zstring::isws(*src))
		++src;

	if (*src == '\0')
		return;

	if ((sep = strchr(src, '|')) == NULL)
		throw zetable("no code separator", in);

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
				throw zetable("unknown translation directive", in);
			atomic = false;
		} else {
			char *unused;
			code = strtol(tok.c_str(), &unused, 16);
		}

		codes.push_back(code);
	}

	if (atomic) {
		mapa.add(zinst(mnemo), codes);
	} else {
		mapv.add(zinst(mnemo), codes);
	}

	if (opt.debug.instab) {
		debug("Installed instruction: %s\n", zinst(mnemo).c_str());

		if (opt.debug.mcode && codes.size() != 0) {
			for (std::vector<int>::const_iterator it = codes.begin(); it != codes.end(); ++it) {
				debug("  data: %c%02X\n", *it >= 0 ? ' ' : '-', abs(*it));
			}
		}
	}
}

void zcpu::init(const zstring &cpu_name)
{
	zstring fname;

	if (strchr(cpu_name.c_str(), '/') == NULL) {
		fname.format(PREFIX "/" SHAREPATH "/cpu/%s", cpu_name.c_str());
	} else {
		fname = cpu_name;
	}

	if (opt.debug.filerd)
		debug("Reading instruction table from \"%s\".\n", fname.c_str());

	zinput in(fname.c_str());
	if (!in.open())
		throw zefile("could not open instruction table for reading", fname.c_str());

	stat.tabtime = gettime();
	for (zstring line; in.read(line); add_instr(line.c_str(), in));
	stat.tabtime = gettime() - stat.tabtime;

	ready = true;

	output.push_back(new zoutput("default"));
}

// Translates all specified input files.  If a file could not be
// opened or no input files were specified, returns false.
void zcpu::translate()
{
	if (input.size() == 0)
		throw zenofiles();

	stat.trantime = gettime();

	while (input.size() != 0) {
		zinput &i = current();
		if (!i.is_open() && !i.open())
			throw zefile("could not open file for reading", i.name());
		if (opt.debug.filerd)
			debug("Translating \"%s\".\n", i.name());
		while (parse(i, *output[iout]));
		input.pop_back();
	}

	stat.trantime = gettime() - stat.trantime;

	for (std::vector<zoutput *>::const_iterator it = output.begin(); it != output.end(); ++it)
		(*it)->write();
}

void zcpu::resolve()
{
	bool repeat, delayed;

	stat.fixtime = gettime();

	do {
		repeat = false;
		delayed = false;

		for (vector<zymbol *>::const_iterator it = symbols.begin(); it != symbols.end(); ++it) {
			if (!(*it)->isok()) {
				delayed = true;
				if ((*it)->evaluate(symbols)) {
					if (opt.debug.symtab)
						debug("Symbol \"%s\" resolved.\n", (*it)->c_str());
					repeat = true;
				}
			}
		}
	} while (repeat);

	if (delayed) {
		bool lock = false;

		for (vector<zymbol *>::const_iterator it = symbols.begin(); it != symbols.end(); ++it) {
			if ((*it)->islabel() && !(*it)->isok()) {
				if (!lock) {
					fprintf(stderr, "Values of the following labels could not be evaluated:\n");
					lock = true;
				}
				fprintf(stderr, " - %s (%s)\n", (*it)->c_str(), (*it)->extra());
			}
		}

		lock = false;

		if (opt.debug.symtab) {
			for (vector<zymbol *>::const_iterator it = symbols.begin(); it != symbols.end(); ++it) {
				if (!(*it)->islabel() && !(*it)->isok()) {
					if (!lock) {
						fprintf(stderr, "The following expressions could not be calculated:\n");
						lock = true;
					}
					fprintf(stderr, " - %s\n", (*it)->c_str());
				}
			}
		}

		throw zemsg("not all expressions could be evaluated");
	} else {
		if (opt.fsym.is_open() && opt.symbols.labels) {
			opt.fsym.print("; Symbol table follows (%u elements)\n", symbols.size());

			for (vector<zymbol *>::const_iterator it = symbols.begin(); it != symbols.end(); ++it) {
				if ((*it)->islabel()) {
					opt.fsym.print(";  %04Xh ", (*it)->get_value() & 0xFFFF);
					opt.fsym.print(";                         ");
					opt.fsym.print("; %s", (*it)->c_str());
					opt.fsym.print("\n");
				}
			}

			opt.fsym.print(";\n");
		}
	}

	stat.fixtime = gettime() - stat.fixtime;
}

bool zcpu::parse(zinput &in, zoutput &out)
{
	zstring label, line;

	if (!in.read(line))
		return false;

	if (opt.debug.lines)
		debug("#line %s:%u\n", in.name(), in.line());

	stat.lines++;

	if (get_label(label, line) && opt.fsym.is_open() && opt.symbols.source) {
		if (opt.symbols.addr)
			opt.fsym.print(";        ");
		if (opt.symbols.code)
			opt.fsym.print(";                         ");
		opt.fsym.print("; ; %s\n", label.c_str());
	}

	if (line.size() != 0) {
		zinst inst(line);
		size_t offset = out.size();

		if (!do_atomic(inst.str(), out) && !do_variable(inst.str(), out, label)) {
			throw zesyntax(line.c_str(), "unknown instruction", &in);
		}

		if (label.size() > 0) {
			symbols.push_back(new zlabel(label.c_str(), "$", &out.block(), offset));
			if (opt.debug.newsym)
				debug("New label: %s.\n", label.c_str());
		}

		// Dump the code.
		if (opt.fsym.is_open()) {
			bool pc_sent = false;
			size_t lim = out.size();

			do {
				bool lf = false;
				bool colon = false;

				if (opt.symbols.addr) {
					lf = true;

					if (offset >= lim)
						opt.fsym.print(";        ");
					else if (out.block().has_origin())
						opt.fsym.print(";  %04Xh ", out.block().get_origin() + offset);
					else
						opt.fsym.print("; +%04Xh ", offset);
				}

				for (size_t idx = 0; idx < 8; ++idx, ++offset) {
					if (opt.symbols.code) {
						lf = true;

						if (!colon) {
							opt.fsym.print("; ");
							colon = true;
						}

						if (offset < lim)
							opt.fsym.print("%02X ", static_cast<unsigned char>(out[offset]));
						else if (opt.symbols.source)
							opt.fsym.print("   ");
					}
				}

				if (!pc_sent) {
					if (opt.symbols.source) {
						lf = true;
						opt.fsym.print("; %s", inst.c_str());
					}
					pc_sent = true;
				} else {
					if (opt.symbols.source) {
						lf = true;
						opt.fsym.print(";");
					}
				}

				if (lf)
					opt.fsym.print("\n");
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

bool zcpu::do_atomic(const zstring &line, zoutput &out)
{
	const std::vector<int> *codes;

	if (!mapa.findx(line, &codes))
		return false;

	for (std::vector<int>::const_iterator bit = codes->begin(); bit != codes->end(); ++bit)
		out.emit(static_cast<char>(*bit));

	return true;
}

bool zcpu::do_variable(const zstring &line, zoutput &out, zstring &label)
{
	const zinst *inst;
	const std::vector<int> *data;
	int base = out.block().get_origin() + out.block().size();

	if (!mapv.findt(line, &data, &inst))
		return false;

	if (data->size() != 0) {
		std::vector<zstring> args;

		inst->get_args(line, &args);

		for (std::vector<int>::const_iterator it = data->begin(); it != data->end(); ++it) {
			if (*it >= 0) {
				out.emit(static_cast<char>(*it));
			} else {
				switch (*it) {
				case op_byte:
				case op_boffset:
					emit(args[0], static_cast<opcode_e>(*it), out, base);
					args.erase(args.begin());
					break;
				case op_word:
					emit(args[0], op_word, out, base);
					args.erase(args.begin());
					break;
				case op_zap:
					args.erase(args.begin());
					break;
				case op_define:
					symbols.push_back(new zlabel(label.c_str(), args.begin()->c_str(), &out.block(), base));
					label.erase();
					break;
				case op_origin:
					if (out.block().size() != 0)
						out.add();
					{
						int value = 0;
						const char *expr = args.begin()->c_str();
						if (zymbol::evaluate(expr, value, 0, symbols)) {
							out.block().set_origin(value);
						} else {
							zorigin *org = new zorigin(expr, &out.block());
							symbols.push_back(org);
							out.block().set_origin(org);
						}
					}
					args.erase(args.begin());
					break;
				case op_include:
					break;
				case op_insert:
					break;
				case op_blist:
					for (const char *src = args[0].c_str(); *src != 0; ) {
						zstring tok = zstring::gettok(src, ',');
						emit(tok, op_byte, out, base);
					}
					args.erase(args.begin());
					break;
				case op_wlist:
					for (const char *src = args[0].c_str(); *src != 0; ) {
						zstring tok = zstring::gettok(src, ',');
						emit(tok, op_word, out, base);
					}
					args.erase(args.begin());
					break;
				default:
					throw zesyntax(line.c_str(), "unsupported parameter", &current());
				}
			}
		}
	}

	return true;
}

void zcpu::emit(const zstring &expr, opcode op, zoutput &out, int base)
{
	int value = 0;
	const char *it = expr.c_str();

	if (!zymbol::evaluate(it, value, out.block().has_origin() ? base : -1, symbols)) {
		value = 0;
		symbols.push_back(new zexpression(expr.c_str(), &out.block(), base, out.block().size(), op));
		if (opt.debug.newsym)
			debug("Expression \"%s\" stored for later evaluation.\n", expr.c_str());
	} else {
		if (op == op_boffset)
			value -= base;

		if (opt.debug.newsym)
			debug("Expression \"%s\" emits data.\n", expr.c_str());
	}

	if (op == op_boffset)
		op = op_byte;

	switch (op) {
	case op_byte:
		if (value < -128 || value > 255)
			throw zesyntax(expr.c_str(), "byte overflow", &current());
		out.emit(static_cast<char>(value));
		break;
	case op_word:
		if (value < -32768 || value > 65535)
			throw zesyntax(expr.c_str(), "word overflow", &current());
		out.emit(static_cast<short>(value));
		break;
	default:
		throw zesyntax(expr.c_str(), "unknown data type (internal error)", &current());
	}
}

size_t zcpu::gettime(void)
{
	size_t sec, msec;
	static size_t base = 0;

#ifdef _WIN32
	SYSTEMTIME st;
	GetSystemTime(&st);
	sec = static_cast<size_t>(st.wSecond);
	msec = static_cast<size_t>(st.wMilliseconds);
#else
	struct timeval tv;
	memset(&tv, 0, sizeof(tv));
	gettimeofday(&tv, NULL);
	sec = static_cast<size_t>(tv.tv_sec);
	msec = static_cast<size_t>(tv.tv_usec / 1000);
#endif

	if (base == 0)
		base = sec;

	return (sec - base) * 1000 + msec;
}

zinput& zcpu::current()
{
	return input[input.size() - 1];
}

void zcpu::append(const char *fname)
{
	input.push_back(zinput(fname));
}
