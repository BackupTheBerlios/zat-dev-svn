// Faerion RunTime Library.
// Copyright (c) 2003-2004 hex@faerion.oss and others.
// Distributed under the terms of GNU LGPL, read 'LICENSE'.
//
// $Id$

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "fileio.h"

static int IsEOL(int c) { return (c == '\r' || c == '\n'); }

char * freadline(char *buf, size_t size, FILE *in)
{
	size_t len = 0;
	char *str = NULL;

	if (!feof(in) && (str = fgets(buf, size, in)) != NULL) {
		len = strcspn(buf, "\r\n");

		if (buf[len] != '\0')
			buf[len] = '\0';

		/* fgets returns truncated string.  if we didn't get
		 * any EOL character, then we couldn't read the whole
		 * string, skip the rest now.
		 */
		else while (!feof(in) && !IsEOL(fgetc(in)));

		return buf;
	}

	return NULL;
}
