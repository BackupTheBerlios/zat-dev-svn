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
			else
				throw zesyntax(src, "unknown machine code extension");
			atomic = false;
		} else {
			char *unused;
			code = strtol(tok.c_str(), &unused, 16);
		}

		codes.push_back(code);
	}

	if (atomic) {
		mapa[zinst(mnemo)] = codes;
		debug(3, "(a) installed instruction '%s'\n", zinst(mnemo).c_str());
	} else {
		std::vector<int> &x = mapv[zinst(mnemo)];
		x = codes;
		opt.fsym.print("; installed command: '%s' (%u):", zinst(mnemo).c_str(), codes.size());
		for (size_t idx = 0; idx < x.size(); ++idx)
			opt.fsym.print(" %02X", x[idx] & 0xFF);
		opt.fsym.print("\n");
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

	if (get_label(label, line)) {
		if (opt.fsym.is_open())
			opt.fsym.print("; label: %s\n", label.c_str());
	}

	if (line.size() != 0) {
		zinst inst(line);
		size_t offset = out.size();

		if (!do_atomic(inst, out) && !do_variable(inst, out)) {
			debug(1, " -- hashes: a=%u, v=%u.\n", inst.hinta, inst.hintv);
			throw zesyntax(line.c_str(), "unknown instruction");
		}

		// Dump the code.
		if (opt.fsym.is_open()) {
			bool pc_sent = false;
			size_t lim = out.size();

			do {
				opt.fsym.print("; %04Xh ", offset);

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

bool zcpu::do_variable(zinst &inst, zoutput &out)
{
	mapv_t::const_iterator it = mapv.find(inst);

	if (it == mapv.end())
		return false;

	if (it->second.size() != 0) {
		const std::vector<int> &codes = it->second;

		debug(1, "- : size=%u\n", codes.size());

		for (std::vector<int>::const_iterator it = codes.begin(); it != codes.end(); ++it) {
			debug(1, "- : %d\n", *it);
			if (*it >= 0) {
				out.emit(static_cast<char>(*it));
			} else {
				switch (*it) {
				case op_byte:
					out.emit(static_cast<char>(0));
					break;
				case op_word:
					out.emit(static_cast<short>(0));
					break;
				}
			}
		}
	}

	return true;
}
