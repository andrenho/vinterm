#include "graphic/fontbmp.h"

#include <iostream>

#include "graphic/framebuffer.h"

#define P(sf,x,y) *((Uint8*)(sf)->pixels + (y)*(sf)->pitch + (x))

FontBMP::FontBMP(string filename, int w, int h, int chars_per_row, 
		string encoding)
	: w(w), h(h), chars_per_row(chars_per_row), encoding(encoding),
	  background(0), foreground(0)
{
	sf = SDL_LoadBMP(filename.c_str());
	if(!sf)
		throw "error: font file " + filename + "not found";
	if(sf->format->BytesPerPixel != 1)
		throw "error: font file must be a 256-color file";
	FindColors();
}


FontBMP::~FontBMP()
{
	if(sf)
		SDL_FreeSurface(sf);
}


void 
FontBMP::DrawChar(uint32_t ch, uint8_t* pixels, int x, int y, int pitch,
		uint8_t bg_color, uint8_t fg_color, bool underline) const
{
	int ch_x = (ch % chars_per_row) * w;
	int ch_y = ch / chars_per_row * h;
	if(ch_x + w - 1 > sf->w || ch_y + h - 1 > sf->h)
		abort();
	for(int xx=0; xx<w; xx++)
		for(int yy=0; yy<h; yy++)
		{
			int p = (x+xx)+(y+yy)*pitch;
			if(p > framebuffer->W() * framebuffer->H())
				break;
			if(P(sf, ch_x+xx, ch_y+yy) == foreground)
				pixels[p] = fg_color;
			else
				pixels[p] = bg_color;
		}
	if(underline)
		for(int xx=0; xx<w; xx++)
		{
			int p = (x+xx)+(y+h-1)*pitch;
			if(p > framebuffer->W() * framebuffer->H())
				break;
			pixels[p] = fg_color;
		}
}


void
FontBMP::FindColors()
{
	background = ((uint8_t*)(sf)->pixels)[0];
	for(int i = 1; i<(sf->w * sf->h); i++)
	{
		uint8_t c = ((uint8_t*)(sf)->pixels)[i];
		if(c != background)
		{
			foreground = c;
			return;
		}
	}
}
