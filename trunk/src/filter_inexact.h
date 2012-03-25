#ifndef FILTER_INEXACT_H
#define FILTER_INEXACT_H

#include <cstdio>
#include "filter.h"

class FilterInexact : public Filter
{
public:
	FilterInexact(int level=30, bool background=false) 
		: level(level), background(background) { }
	~FilterInexact() { }
	void Apply(SDL_Surface* sf, Options const& opt) const;
	inline FilterType Type() const { return PRE; }

private:
	const int level;
	const bool background;
};

#endif
