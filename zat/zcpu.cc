// ZAA, ZX Assembler assembler (umm).
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$

#include <stdio.h>
#include <stdlib.h>

#include "zcpu.h"
#include "zerror.h"

zcpu cpu;

zcpu::zcpu()
{
	fname = 0;
	line = 0;
	addr = 0;
	lastlabel = NULL;
	caddr = 0;

	ramsize = 64 * 1024;
	ramdata = reinterpret_cast<unsigned char *>(malloc(ramsize));
}


zcpu::~zcpu()
{
	if (ramdata != NULL)
		free(ramdata);
}


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


void zcpu::initcom()
{
	lastlabel = NULL;
	caddr = addr;
	usemap = ~0;
}


void zcpu::unuse(unsigned int N)
{
	unsigned int idx = addr - caddr - N;

	while (idx < 32 && N != 0) {
		usemap &= ~(1 << idx);
		--N;
		++idx;
	}
}
