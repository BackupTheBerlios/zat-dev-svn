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
	const char *sep;
	vector<int> codes;

	while (zstring::isws(*src))
		++src;

	if (*src == ';')
		return;

	if (( sep = strchr(src, '|')) == NULL)
		throw zesyntax(src, "malformed instruction table");

	debug("adding instruction: %s\n", std::string(src, sep - src).c_str());
}

void zcpu::init(const char *cpu_name)
{
	zstream in;
	std::string fname, line;

	if (strchr(cpu_name, '/') == NULL) {
		fname = PREFIX "/" SHAREPATH "/cpu/" + std::string(cpu_name);
	} else {
		fname = cpu_name;
	}

	in.open(fname.c_str());

	if (!in.is_open())
		throw zefile("could not open instruction table for reading", fname.c_str());

	for (std::string line; in.read(line); add_instr(line.c_str()));

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
		debug("translating \"%s\".\n", i.name());
		while (i.do_line(*output[iout]));
		input.pop_back();
	}
}
