// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <ctype.h>
#include <string.h>

#include "zsupport.h"

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
