// Faerion RunTime Library.
// Copyright (c) 2002-2004 hex@faerion.oss and others.
// $Id$

#include "timespec.h"

using namespace faeutil;

timespec::timespec(unsigned int _msec)
{
	if (_msec == ~0U)
		update();
	else
		*this = _msec;
}


timespec::timespec(const timespec &src)
{
	sec = src.sec;
	msec = src.msec;
}


void timespec::update()
{
	faeutil_gettime(&sec, &msec);
}


timespec& timespec::operator += (const timespec &src)
{
	sec += src.sec;
	msec += src.msec;
	if (msec >= 1000) {
		sec += msec / 1000;
		msec %= 1000;
	}
	return *this;
}


timespec& timespec::operator -= (const timespec &src)
{
	sec -= src.sec;
	if (msec < src.msec) {
		msec += 1000;
		--sec;
	}
	msec -= src.msec;
	return *this;
}


timespec& timespec::operator = (const timespec &src)
{
	sec = src.sec;
	msec = src.msec;
	return *this;
}


bool timespec::operator < (const timespec &src)
{
	if (sec < src.sec)
		return true;
	if (sec == src.sec && msec < src.msec)
		return true;
	return false;
}


bool timespec::operator > (const timespec &src)
{
	if (sec > src.sec)
		return true;
	if (sec == src.sec && msec > src.msec)
		return true;
	return false;
}


timespec& timespec::operator = (unsigned int _msec)
{
	sec = _msec / 1000;
	msec = _msec % 1000;
	return *this;
}


timespec timespec::operator - (const timespec &src)
{
	timespec tmp(*this);
	tmp -= src;
	return tmp;
}


timespec timespec::operator + (unsigned int _msec)
{
	timespec tmp(*this);
	tmp += _msec;
	return tmp;
}


unsigned int timespec::mseconds()
{
	return sec * 1000 + msec;
}
