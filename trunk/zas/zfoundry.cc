// Zat Assembler Toolchain.
// Copyright (C) 2004-2005 Justin Forest <justin.forest@gmail.com>
//
// $Id$

#include "zas.h"
#include "zfoundry.h"

zfoundry::zfoundry(size_t nsize)
{
	size = nsize;
	lines = new line[nsize];
}

zfoundry::~zfoundry()
{
	if (lines != NULL)
		delete [] lines;
}

void zfoundry::add(const zinst &key, const data &val)
{
	size_t weight = key.get_weight();
	line &l = lines[key.hash() % size];
	line::iterator pos = l.begin();

	while (pos != l.end() && weight > pos->first.get_weight())
		++pos;

	l.insert(pos, element(key, val));
}

bool zfoundry::find(const zstring &src, const data **d, const zinst **i, bool atomic)
{
	line *l;
	unsigned int hval;
	zstring text(src);

	zinst::optimize(text);
	hval = zinst::calc_hash(text, atomic);
	l = &lines[hval % size];

	for (line::const_iterator it = l->begin(); it != l->end(); ++it) {
		if (it->first.hash() != hval)
			continue;

		if (atomic ? it->first.matchx(text) : it->first.matcht(text)) {
			if (d != NULL)
				*d = &it->second;
			if (i != NULL)
				*i = &it->first;
			return true;
		}
	}

	return false;
}
