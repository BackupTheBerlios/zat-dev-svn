// Faerion RunTime Library.
// Copyright (c) 2003-2004 hex@faerion.oss and others.
// Distributed under the terms of GNU LGPL, read 'LICENSE'.
//
// $Id$

#ifndef __faeutil_time_h
#define __faeutil_time_h

#if defined(__cplusplus)

namespace faeutil
{

class timespec
{
public:
	unsigned int sec, msec;
	timespec(unsigned int _msec = ~0U);
	timespec(const timespec &src);
	timespec& operator += (const timespec &src);
	timespec& operator -= (const timespec &src);
	timespec& operator = (const timespec &src);
	timespec& operator = (unsigned int _msec);
	bool operator < (const timespec &src);
	bool operator > (const timespec &src);
	timespec operator - (const timespec &a);
	timespec operator + (unsigned int _msec);
	void update();
	unsigned int mseconds();
};

};

#define _futexport extern "C"
#else
# define _futexport
#endif

_futexport void faeutil_gettime(unsigned int *sec, unsigned int *msec);

#endif // __faeutil_time_h
