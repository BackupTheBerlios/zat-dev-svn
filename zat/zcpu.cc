// Zat Assembler Toolchain.
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$

#include <stdio.h>
#include <stdlib.h>

#include "zcpu.h"
#include "zerror.h"
#include "zinst.h"
#include "zoptions.h"

zcpu cpu;

zcpu::zcpu()
{
}

zcpu::~zcpu()
{
}

zerror zcpu::init(const char *cpu_name)
{
	FILE *in;
	char buf[4096], *src;

	if (strchr(cpu_name, '/') == NULL)
		snprintf(buf, sizeof(buf), PREFIX "/" SHAREPATH "/cpu/%s", cpu_name);
	else
		strncpy(buf, cpu_name, sizeof(buf));

	debug("reading the translation table from %s\n", buf);

	if ((in = fopen(buf, "rb")) == NULL)
		return ret_bad_cpu_table;

	while ((src = read_line(buf, sizeof(buf), in)) != NULL)
		zinst::feed(buf);

	fclose(in);

	debug("optimizing the translation table.\n");
	zinst::optimize();

	debug("translation table read ok.\n");
	return ret_ok;
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
zerror zcpu::translate(int argc, char * const *argv)
{
	while (argc != 0) {
		input.push_back(zinput(*argv));
		if (!input[input.size() - 1].open()) {
			debug("could not read from \"%s\".\n", *argv);
			return ret_inerr;
		}
		--argc, ++argv;
	}

	if (input.size() == 0) {
		return ret_no_input_files;
	}

	while (input.size() != 0) {
		zinput &i = input[input.size() - 1];
		debug("translating \"%s\".\n", i.name());
		while (i.do_line(*output[iout]))
			;
		input.pop_back();
	}

	debug("translation finished.\n");
	return ret_ok;
}
