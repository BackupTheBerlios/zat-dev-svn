// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "zymbol.h"

zymbol::zymbol()
{
	value = 0;
	block = 0;
	base = 0;
	ready = false;
}

zymbol::zymbol(const char *lname) :
	text(lname)
{
	value = 0;
	block = 0;
	base = 0;
	ready = false;
}

zymbol::~zymbol()
{
}

bool zymbol::evaluate(const char *&expr, int &value, int base, list &symbols)
{
	int tval = 0;
	char sign = '+';

	while (*expr != '\0') {
		if (*expr == '-' || *expr == '+' || *expr == '*' || *expr == '/') {
			sign = *expr++;
		}

		else if (sign == 0) {
			return false;
		}

		else if (*expr == '(') {
			++expr;

			if (!evaluate(expr, tval, base, symbols))
				return false;

			if (*expr != ')')
				return false;

			++expr;
		}

		else if (*expr == '#') {
			++expr;
			tval = get_hex(expr);
		}

		else if (*expr == '%') {
			++expr;
			tval = get_bin(expr);
		}

		else if (*expr == '$') {
			if (base < 0)
				return false;
			++expr;
			tval = base;
		}

		else if (isdigit(*expr)) {
			tval = get_dec(expr);
		}

		else {
			return false;
		}

		switch (sign) {
		case '+':
			value += tval;
			break;
		case '-':
			value -= tval;
			break;
		case '*':
			value *= tval;
			break;
		case '/':
			if (tval == 0)
				return false;
			value /= tval;
			break;
		}
	}

	return true;
}

int zymbol::get_hex(const char *&e)
{
	int value = 0;

	while (*e != 0) {
		if (*e >= '0' && *e <= '9')
			value = value * 16 + *e - '0';
		else if (*e >= 'a' && *e <= 'f')
			value = value * 16 + *e - 'a' + 10;
		else if (*e >= 'A' && *e <= 'F')
			value = value * 16 + *e - 'A' + 10;
		else {
			break;
		}

		++e;
	}

	return value;
}

int zymbol::get_bin(const char *&e)
{
	int value = 0;

	while (*e != 0) {
		if (*e == '0')
			value = value * 2 + 0;
		else if (*e == '1')
			value = value * 2 + 1;
		else {
			value = 0;
			break;
		}
	}

	return value;
}

int zymbol::get_dec(const char *&e)
{
	int value = 0;
	const char *base = e;

	while (*e != 0) {
		if (*e >= '0' && *e <= '9')
			value = value * 10 + *e - '0';
		else if (*e == 'h' || *e == 'H') {
			e = base;
			value = get_hex(e);
			++e;
			break;
		} else {
			break;
		}
		++e;
	}

	return value;
}
