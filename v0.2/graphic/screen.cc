#include "graphic/screen.h"

#include <cstdlib>
#include <set>
#include "SDL.h"
using namespace std;

#include "options.h"
#include "terminal/framebuffer.h"
#include "graphic/chars.h"
#include "graphic/font.h"

Screen::Screen(Options const& options, Framebuffer const& fb)
	: options(options), font(new Font()), 
	  chars(new Chars(options, *font)), fb(fb),
	  border_x(options.border_x * options.scale),
	  border_y(options.border_y * options.scale),
	  w(fb.W() * font->char_w * options.scale + (border_x * 2)), 
	  h(fb.H() * font->char_h * options.scale + (border_y * 2))
{
       	screen = SDL_SetVideoMode(w, h, 8, SDL_SWSURFACE);
	if(!screen)
	{
		fprintf(stderr, "It was not possible to open the display: %s\n",
				SDL_GetError());
		exit(1);
	}
	SDL_WM_SetCaption("Vintage Emulator " VERSION, "Vintage Emulator");

	SDL_EnableUNICODE(1);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, 
			SDL_DEFAULT_REPEAT_INTERVAL);

	initializePalette(screen, options);
}


Screen::~Screen()
{
	delete font;
	delete chars;
}


void
Screen::initializePalette(SDL_Surface* sf, Options const& options)
{
	double rr((options.bright_color.r - options.background_color.r) / 255.0);
	double rg((options.bright_color.g - options.background_color.g) / 255.0);
	double rb((options.bright_color.b - options.background_color.b) / 255.0);

	SDL_Color palette[256];
	for(double i(0); i<256; i++)
		palette[(Uint8)i] = SDL_Color {
			(Uint8)(options.background_color.r + (rr * i)),
			(Uint8)(options.background_color.g + (rg * i)),
			(Uint8)(options.background_color.b + (rb * i)) };
	SDL_SetColors(sf, palette, 0, 256);
}


void
Screen::Update()
{
	vector<SDL_Rect> rects;
	int i(0);

	// update data from the framebuffer
	set<int>::const_iterator n;
	for(n = fb.dirty->begin(); n != fb.dirty->end(); n++)
	{
		int x((*n) % fb.W());
		int y((*n) / fb.W());
		Char ch(fb.Ch(x, y));
		int xx((x * font->char_w * options.scale) + border_x 
			- (options.scale * chars->start_at_x));
		int yy((y * font->char_h * options.scale) + border_y
			- (options.scale * chars->start_at_y));
		//Attribute attr(ch.cursor ? (CharAttr) { 1 } : ch.attr);
		Attribute attr = ch.Attr;
		SDL_Surface* sf(chars->Char(ch.Ch, attr, x+y));
		if(!sf)
			abort();
		SDL_Rect r = { 
			(Sint16)xx, (Sint16)yy, 
			(Sint16)sf->w, (Sint16)sf->h 
		};

		// clear char
		SDL_Rect r2 = { 
			(Sint16)(xx+(options.scale * chars->start_at_y)),
			(Sint16)(yy+(options.scale * chars->start_at_y)),
			(Sint16)(font->char_w * options.scale),
			(Sint16)(font->char_h * options.scale+1)
		};
		if(x == 0) { r2.x-=2; r2.w+=2; } else if(x == (w-1)) { r2.w+=2; }
		if(y == 0) { r2.y-=2; r2.h+=2; } else if(y == (h-1)) { r2.h+=2; }
		SDL_FillRect(screen, &r2, 0);

		// draw char
//		if(!((ch.Attr.Blink || ch.cursor) && !terminal.BlinkOn()))
			SDL_BlitSurface(sf, NULL, screen, &r);
/*		else if(ch.cursor && !terminal.BlinkOn())
		{
			// show underletter when blinking is off
			SDL_Surface* sf2(chars->Char(ch.ch, ch.attr, x+y));
			SDL_BlitSurface(sf2, NULL, screen, &rects[i]);
		} */
		rects.push_back(r);

	//	SDL_Flip(screen);

		i++;
	}
	fb.dirty->clear();

	// update screen
	SDL_UpdateRects(screen, i, &rects[0]);
}


int
Screen::Input() const
{
	// read chars and send them to the console
	SDL_Event e;
	uint16_t c;

	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
		case SDL_KEYDOWN:
			c = e.key.keysym.unicode;
			if(c != 0)
				return c;
			break;
		case SDL_QUIT:
			return QUIT;
		}
	}
	return 0;
}
