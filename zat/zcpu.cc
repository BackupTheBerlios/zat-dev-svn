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

		if (* tok.c_str() == '@') {
			if (tok == "@byte")
				code = op_byte;
			else if (tok == "@word")
				code = op_word;
			else if (tok == "@boffset")
				code = op_boffset;
			else if (tok == "@zap")
				code = op_zap;
			else if (tok == "@equ")
				code = op_equ;
			else if (tok == "@org")
				code = op_org;
			else if (tok == "@include")
				code = op_include;
			else if (tok == "@insert")
				code = op_insert;
			else
				throw zesyntax(src, "unknown machine code extension");
			atomic = false;
		} else {
			char *unused;
			code = strtol(tok.c_str(), &unused, 16);
		}

		codes.push_back(code);
	}

	if (atomic)
		mapa[zinst(mnemo)] = codes;
	else
		mapv[zinst(mnemo)] = codes;
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

	output.push_back(new zoutput(opt.out));
}

/*
zerror zcpu::emit_b(int data, unsigned int pc)
{
	if (data > 0xFF)
		return ret_byte_overflow;
	if (data < -0x80)
		return ret_byte_overflow;
	ramdata[pc] = data;
	return ret_ok;
}

zerror zcpu::emit_b(int data)
{
	return emit_b(data, addr++);
}

zerror zcpu::emit_w(int data, unsigned int at)
{
	if (data > 0xFFFF)
		return ret_word_overflow;
	if (data < -0x8000)
		return ret_word_overflow;
	ramdata[at+0] = data & 0xFF;
	ramdata[at+1] = (data >> 8) & 0xFF;
	return ret_ok;
}

zerror zcpu::emit_w(int data)
{
	zerror rc = emit_w(data, addr);
	addr += 2;
	return rc;
}
*/


void zcpu::initcom()
{
	/*
	lastlabel = NULL;
	caddr = addr;
	usemap = ~0;
	*/
}


void zcpu::unuse(unsigned int /*N*/)
{
	/*
	unsigned int idx = addr - caddr - N;

	while (idx < 32 && N != 0) {
		usemap &= ~(1 << idx);
		--N;
		++idx;
	}
	*/
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
		debug(2, " - label: %s\n", label.c_str());
	}

	if (line.size() != 0) {
		zinst inst(line);
		if (!do_atomic(inst, out) && !do_variable(inst, out)) {
			debug(1, " -- hashes: a=%u, v=%u.\n", inst.hinta, inst.hintv);
			throw zesyntax(line.c_str(), "unknown instruction");
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

bool zcpu::do_atomic(zinst &inst, zoutput &)
{
	mapa_t::const_iterator it = mapa.find(inst);

	if (it == mapa.end()) {
		debug(1, " - '%s' not found in atomics.\n", inst.c_str());
		return false;
	}

	debug(1, " - found an atomic command: '%s'\n", inst.c_str());
	return true;
}

bool zcpu::do_variable(zinst &inst, zoutput &)
{
	mapv_t::const_iterator it = mapv.find(inst);

	if (it == mapv.end()) {
		debug(1, " - '%s' not found in variables.\n", inst.c_str());
		return false;
	}

	debug(1, " - found a variable command: '%s'\n", inst.c_str());
	return true;
}
