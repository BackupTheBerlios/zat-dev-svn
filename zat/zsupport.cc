// Zat Assembler Toolchain.
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "zsupport.h"

char *read_line(char *buf, size_t size, FILE *in)
{
	char *str;
	size_t idx;

	str = NULL;

	if (feof(in))
		return NULL;

	if (!feof(in) && (str = fgets(buf, size, in)) != NULL) {
		if (buf[idx = strcspn(buf, "\r\n")])
			buf[idx] = '\0';
		/* fgets returns truncated string.  if we didn't get
		 * any EOL character, then we couldn't read the whole
		 * string, read the rest now.
		 */
		else while (!feof(in) && !IsEOL(fgetc(in)));
	}

	return str;
}


void strtoupper(char *src)
{
	while (*src != '\0') {
		if (islower(*src))
			*src = toupper(*src);
		++src;
	}
}


char * zatok(char *str, char sep, char **save)
{
	char *pos = *save, *tmp;

	if (str != NULL)
		pos = str;

	if (pos == NULL || *pos == '\0')
		return (*save = NULL);

	tmp = pos;

	while (*pos != '\0' && *pos != sep)
		++pos;

	if (*pos != '\0')
		*pos++ = '\0';
	else
		pos = NULL;

	*save = pos;
	return tmp;
}
