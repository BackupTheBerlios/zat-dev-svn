// ZAA, ZX Assembler assembler (umm).
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$

#ifndef __zaa_zsupport_h
#define __zaa_zsupport_h

extern char * read_line(char *, size_t, FILE *);
extern void strtoupper(char *src);
extern unsigned int crc32(const void *src, size_t size);
extern char * zatok(char *str, char sep, char **save);

static inline bool IsEOL(int c) { return (c == '\r' || c == '\n'); }
static inline bool IsWS(int c) { return (c == ' ' || c == '\t'); }

static inline int htoi(char value)
{
	if (value >= '0' && value <= '9')
		return value - '0';
	if (value >= 'A' && value <= 'F')
		return value - 'A' + 10;
	return 0;
}

#endif // __zaa_zsupport_h
