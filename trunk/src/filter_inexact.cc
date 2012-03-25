#include "filter_inexact.h"

#include <cstdlib>
#include <algorithm>
using namespace std;

void 
FilterInexact::Apply(SDL_Surface* sf, Options const& opt) const
{
	for(int x=0; x<sf->w; x++)
		for(int y=0; y<sf->h; y++)
		{
			int v = P(sf, x, y);
			if(v > 10 || background)
			{
				int n = (rand() % (level * 2)) - level;
				v = min(max(v+n, 0), 255);
				P(sf, x, y) = v;
			}
		}
}
