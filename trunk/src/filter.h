#ifndef FILTER_H
#define FILTER_H

#include <cstdio>
#include "screen.h"

typedef enum { PRE, POST } FilterType;

class Filter
{
public:
	virtual ~Filter() { }
	virtual void Apply(SDL_Surface* sf, Options const& opt) const = 0;
	virtual FilterType Type() const = 0;
};

#endif
