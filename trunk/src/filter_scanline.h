#ifndef FILTER_SCANLINE_H
#define FILTER_SCANLINE_H

#include <cstdio>
#include "filter.h"

class FilterScanline : public Filter
{
public:
	FilterScanline(double level=.75, int each=2) 
		: level(level), each(each) { }
	~FilterScanline() { }
	void Apply(SDL_Surface* sf, Options const& opt) const;
	inline FilterType Type() const { return PRE; }

private:
	const double level;
	const int each;
};

#endif
