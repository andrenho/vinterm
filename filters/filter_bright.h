#ifndef FILTER_BRIGHT_H
#define FILTER_BRIGHT_H

#include <cstdio>
#include "filter.h"

class FilterBright : public Filter
{
public:
	FilterBright(double brightness=1, int sharpness=0) 
		: brightness(brightness), sharpness(sharpness) { }
	~FilterBright() { }

	void Apply(SDL_Surface* const sf, Options const& opt) const;
	inline FilterType Type() const { return PRE; }

private:
	void ApplyPixel(SDL_Surface* const sf, const int x, const int y, 
			const int sp, uint8_t* bsf) const;

	const double brightness;
	const int sharpness;
};

#endif

