// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>

#include "zcpu.h"
#include "zinst.h"
#include "zmemblk.h"
#include "zoptions.h"
#include "zstring.h"
#include "zymbol.h"
#include "zesymbol.h"

std::vector<zymbol> zymbol::list;


zymbol::zymbol(const char *src, int val)
{
	name = src;
	value = val;
}


zymbol::zymbol(const zymbol &src)
{
	*this = src;
}


zymbol::~zymbol()
{
}


zymbol& zymbol::operator = (const zymbol &src)
{
	name = src.name;
	value = src.value;
	bytes = src.bytes;
	words = src.words;
	offsets = src.offsets;
	return *this;
}


bool zymbol::operator < (const zymbol &src) const
{
	return (strcmp(name.c_str(), src.name.c_str()) < 0);
}


void zymbol::dump()
{
	if (opt.fsym.is_open()) {
		std::sort(list.begin(), list.end());

		opt.fsym.print("; Symbol table follows, total symbols: %u.\n", list.size());

		for (iterator it = list.begin(); it != list.end(); ++it) {
			int length = 16 - strlen(it->name.c_str());

			opt.fsym.print("%s", it->name.c_str());
			while (length > 0) {
				opt.fsym.print("\t");
				length -= 8;
			}

			if (it->bytes.size() != 0 || it->words.size() != 0 || it->offsets.size() != 0)
				opt.fsym.print("EQU\t%04Xh\t; undefined (%u references)\n", it->value, it->bytes.size() + it->words.size());
			else
				opt.fsym.print("EQU\t%04Xh\t; %u\n", it->value, it->value);
		}
	}
}


zymbol* zymbol::find(const char *expr)
{
	zname name(expr);
	name.capsize();

	for (iterator it = list.begin(); it != list.end(); ++it) {
		if (name == it->name) {
			return &(*it);
		}
	}

	return NULL;
}


void zymbol::emit()
{
	// Emit all byte sized values.
	for (std::vector<unsigned int>::iterator it = bytes.begin(); it != bytes.end(); ++it) {
		// cpu.emit_b(value, *it);
	}
	bytes.clear();

	// Emit all word sized values.
	for (std::vector<unsigned int>::iterator it = words.begin(); it != words.end(); ++it) {
		// cpu.emit_w(value, *it);
	}
	words.clear();

	// Emit all offset values.
	for (std::vector<unsigned int>::iterator it = offsets.begin(); it != offsets.end(); ++it) {
		// cpu.emit_b(value, *it);
	}
	offsets.clear();
}

void zymbol::install(const char *expr, const zmemblk &em)
{
	install(expr, em.size(), true);
}

void zymbol::install(const char *expr, int value, bool delayed)
{
	zymbol *old;
	zstring name(expr);

	name.capsize();

	if ((old = find(name.c_str())) != NULL) {
		if (delayed)
			return;

		if (!old->is_delayed()) {
			throw zesymbol();
		} else {
			old->value = value;
			old->emit();

			// Do not add to the list.
			return;
		}
	}

	list.push_back(zymbol(name.c_str(), value));

	if (cpu.lastlabel == NULL)
		cpu.lastlabel = &(*(list.end() - 1));
}


const int& zymbol::get_value() const
{
	return value;
}


int& zymbol::get_value()
{
	return value;
}


void zymbol::delay(const char *expr, unsigned int pc, int mode)
{
	zymbol *lab = find(expr);

	if (lab == NULL) {
		install(expr, 0, true);
		lab = &(*(list.end() - 1));
	}

	switch (mode) {
	case op_byte:
		lab->bytes.push_back(pc);
		break;
	case op_word:
		lab->words.push_back(pc);
		break;
	case op_offset:
		lab->offsets.push_back(pc);
		break;
	default:
		// zerror(ret_bad_label_type).repin(expr);
		;
	}
}


bool zymbol::is_delayed() const
{
	if (bytes.size() != 0)
		return true;
	if (words.size() != 0)
		return true;
	if (offsets.size() != 0)
		return true;
	return false;
}


bool zymbol::is_label() const
{
	const char *tmp = name.c_str();

	if (isdigit(*tmp))
		return false;

	while (isalnum(*tmp) || *tmp == '_')
		++tmp;

	return (*tmp == '\0');
}


void zymbol::rescan()
{
	/*
	bool redo = true;

	while (redo) {
		redo = false;

		if (opt.debug)
			fprintf(stderr, "rescanning delayed labels...\n");

		for (iterator it = list.begin(); it != list.end(); ++it) {
			if (it->is_delayed()) {
				zstring name(it->name);

				if (zinst::evaluate(name, it->get_value()) == ret_ok) {
					if (opt.debug >= 2) {
						// fprintf(stderr, "%s:%u: delayed label %s resolved to %04Xh\n", cpu.fname, cpu.line, it->name.c_str(), it->get_value());
					}

					it->emit();

					if (!it->is_delayed()) {
						size_t pos = it - list.begin();
						list.erase(it);
						it = list.begin() + pos - 1;
						redo = true;
					} else {
						fprintf(stderr, "WARNING: sticky delayed label %s.\n", it->name.c_str());
					}
				}
			}
		}
	}

	for (iterator it = list.begin(); it != list.end(); ++it) {
		if (it->is_delayed()) {
			return ret_undefined_symbols;
		}
	}
	*/
}
