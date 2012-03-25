#include "options.h"

#include <memory>
using namespace std;

#include "filter_inexact.h"
#include "filter_scanline.h"
#include "filter_bright.h"

Options::Options() 
	: scale(2), w(80), h(25), border_x(30), border_y(40),
	  background_color((SDL_Color) { 30, 30, 30 }),
	  bright_color((SDL_Color) { 140, 255, 190 }),
	  blink_speed(500)
{
	prefilters.push_back(new FilterBright(2,0));
	if(scale == 1)
	{
		prefilters.push_back(new FilterInexact(20));
	}
	else
	{
		prefilters.push_back(new FilterScanline());
		prefilters.push_back(new FilterInexact(30));
	}
}


Options::~Options()
{
	vector<Filter*>::reverse_iterator filter;
	for(filter = prefilters.rbegin(); filter < prefilters.rend(); filter++)
		delete (*filter);
	prefilters.clear();
}
