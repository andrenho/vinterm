#include "filter_bright.h"

#include <cstdlib>
#include <algorithm>
using namespace std;


void 
FilterBright::Apply(SDL_Surface* const sf, Options const& opt) const
{
	int sp((6-opt.scale)*2 + sharpness);

	uint8_t* bsf(new uint8_t[sf->w * sf->h]);
	for(int x(0); x<sf->w; x++)
		for(int y(0); y<sf->h; y++)
			ApplyPixel(sf, x, y, sp, bsf);

	for(int x(0); x<sf->w; x++)
		for(int y(0); y<sf->h; y++)
			P(sf, x, y) = bsf[(y*sf->w)+x];

	//memcpy(sf->pixels, bsf, sf->w*sf->h);

	delete[] bsf;
}


void 
FilterBright::ApplyPixel(SDL_Surface* const sf, const int x, const int y, 
		const int sp, uint8_t* bsf) const
{
	int c(P(sf, x, y));

	static struct { int x,y; } dirs[] = { 
		{-1,-1},{-1,0},{-1,1},
		{0,-1},{0,1},
		{1,-1},{1,0},{1,1}
	};

	// calculate light
	int total_light(0);
	for(int i(0); i<8; i++)
	{
		int fx(x + dirs[i].x);
		int fy(y + dirs[i].y);
		if(fx < 0 || fy < 0 || fx >= sf->w || fy >= sf->h)
			continue;
		total_light += P(sf, fx, fy);
	}

	// add sharpness
	total_light += P(sf, x, y) * sp;
	c = (total_light) / (7 + sp);

	// apply brightness
	if(c > 50)
		c += (int)(brightness * 10);
	bsf[(y*sf->w)+x] = max(min(c, 255), 0);
}
