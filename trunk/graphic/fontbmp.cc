#include "graphic/fontbmp.h"

#include "SDL2/SDL.h"

#define P(sf,x,y) *((Uint8*)(sf)->pixels + (y)*(sf)->pitch + (x))

FontBMP::FontBMP(string filename, int w, int h, int chars_per_row, 
		string encoding)
	: w(w), h(h), encoding(encoding)
{
	SDL_Surface* sf = SDL_LoadBMP(filename.c_str());
	if(!sf)
		throw "error: font file " + filename + "not found";
}


FontBMP::~FontBMP()
{
}


void 
FontBMP::DrawChar(uint32_t ch, Framebuffer& fb, int x, int y) const
{
}
