// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$
//
// This header defines the base class for an error, as well as all
// derived specific error types.  Might as well be used as exception,
// later.  (Most gcc implementations use sjlj exceptions, which is
// quite slow, and it'd require zat to use objects for everything,
// like file operations and stuff, which I don't think I'm ready
// to do now.)

#ifndef __zat_zerror_h
#define __zat_zerror_h

// Error severities.
typedef enum zseverity_e
{
	zsev_ok,
	zsev_warning,
	zsev_error,
} zseverity;

// The base class corresponds to a no-error situation.
class zerror
{
public:
	// Initialization.
	zerror() { }
	virtual ~zerror() { }
	// Returns the description of the error.
	virtual const char * c_str();
	// Returns the severity of the situation.
	virtual zseverity severity() const { return zsev_error; }
};

class zemsg : public zerror
{
	char *msg;
public:
	zemsg(const char *msg);
	~zemsg();
	const char * c_str();
};

#endif // __zat_zerror_h
