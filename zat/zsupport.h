// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#ifndef __zaa_zsupport_h
#define __zaa_zsupport_h

extern void strtoupper(char *src);
extern unsigned int crc32(const void *src, size_t size);
extern char * zatok(char *str, char sep, char **save);

static inline int htoi(char value)
{
	if (value >= '0' && value <= '9')
		return value - '0';
	if (value >= 'A' && value <= 'F')
		return value - 'A' + 10;
	return 0;
}

#endif // __zaa_zsupport_h
