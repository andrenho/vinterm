#include "font.h"

#include "SDL.h"

#define P(sf,x,y) *((Uint8*)(sf)->pixels + (y)*(sf)->pitch + (x))

Font::Font()
	: char_w(9), char_h(14), ch(new uint8_t*[256]), Encoding("ibm850")
{
	SDL_Surface* sf(SDL_LoadBMP(DATADIR "/850.bmp"));
	if(!sf)
	{
		sf = SDL_LoadBMP("data/850.bmp");
		if(!sf)
		{
			fprintf(stderr, "File 850.bmp could not be found.\n");
			exit(1);
		}
	}

	int x(0), y(0);
	for(int i(0); i<256; i++)
	{
		ch[i] = new uint8_t[char_w*char_h];
		for(int xx(0); xx<char_w; xx++)
			for(int yy(0); yy<char_h; yy++)
				ch[i][yy*char_w+xx] = (P(sf,x+xx,y+yy) == 1 ? 0 : 1);
		x += char_w;
		if(x >= sf->w)
		{
			x = 0;
			y += char_h;
		}
	}

	SDL_FreeSurface(sf);
}


Font::~Font()
{
	for(int i(0); i<256; i++)
		delete[] ch[i];
	delete[] ch;
}
