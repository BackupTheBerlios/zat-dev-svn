// Zat Assembler Toolchain.
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "zat.h"
#include "zstring.h"

zstring::zstring()
{
	data = NULL;
}


zstring::zstring(const char *src)
{
	data = strdup(src);
}


zstring::zstring(const char *src, size_t size)
{
	data = reinterpret_cast<char *>(malloc(size + 1));
	strncpy(data, src, size);
	*(data + size) = '\0';
}


zstring::zstring(const zstring &src)
{
	data = NULL;
	*this = src;
}


zstring::~zstring()
{
	if (data != NULL)
		free(data);
}


zstring& zstring::operator = (const zstring &src)
{
	if (data != NULL)
		free(data);
	data = reinterpret_cast<char *>(strdup(src.data));
	return *this;
}


char * zstring::c_str()
{
	return data;
}


const char * zstring::c_str() const
{
	return data;
}


void zstring::capsize()
{
	for (char *tmp = data; *tmp != '\0'; ++tmp)
		*tmp = ::toupper(*tmp);
}


unsigned int zstring::hint() const
{
	unsigned int value = 0;

	if (data == NULL)
		return value;

	for (const char *src = data; *src != '\0'; ++src)
		value = value * 31 + * reinterpret_cast<const unsigned char *>(src);

	return value;
}

bool zstring::has_path() const
{
#ifdef _WIN32
	if (strchr(data, '\\') != NULL)
		return true;
#endif
	if (strchr(data, '/') != NULL)
		return true;
	return false;
}
