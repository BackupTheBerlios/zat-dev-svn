// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#ifndef __zas_zconst_h
#define __zas_zconst_h

// Special values for machine code.
typedef enum opcode_e
{
	op_byte = -1,
	op_word = -2,
	op_boffset = -3,
	op_zap = -4,
	op_define = -5,
	op_origin = -6,
	op_include = -7,
	op_insert = -8,
	op_blist = -9,
	op_wlist = -10,
	op_namespace = -11,
} opcode;

#endif
