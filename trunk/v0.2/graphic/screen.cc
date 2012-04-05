#include "screen.h"

#include <cstdlib>
#include "SDL.h"

#include "config.h"

Screen::Screen(Options const& options, Terminal& terminal)
	: options(options), 
	  font(new Font()), 
	  chars(new Chars(options, *font)),
	  terminal(terminal), 
	  border_x(options.border_x * options.scale),
	  border_y(options.border_y * options.scale),
	  w(terminal.w * font->char_w * options.scale + (border_x * 2)), 
	  h(terminal.h * font->char_h * options.scale + (border_y * 2)),
	  rects(new SDL_Rect[terminal.w * terminal.h])
{
       	screen = SDL_SetVideoMode(w, h, 8, SDL_SWSURFACE);
	if(!screen)
	{
		fprintf(stderr, "It was not possible to open the display: %s\n",
				SDL_GetError());
		exit(1);
	}
	SDL_WM_SetCaption("Vintage Emulator " PACKAGE_VERSION, 
			"Vintage Emulator");
	initializePalette(screen, options);
}


Screen::~Screen()
{
	delete[] rects;
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
		palette[(Uint8)i] = (SDL_Color) {
			(Uint8)(options.background_color.r + (rr * i)),
			(Uint8)(options.background_color.g + (rg * i)),
			(Uint8)(options.background_color.b + (rb * i)) };
	SDL_SetColors(sf, palette, 0, 256);
}


void
Screen::Update()
{
	int i(0);

	// update data from the terminal
	set<int>::const_iterator n;
	for(n = terminal.dirty.begin(); n != terminal.dirty.end(); n++)
	{
		int x((*n) % terminal.w);
		int y((*n) / terminal.w);
		TerminalChar ch(terminal.Ch(x, y));
		int xx((x * font->char_w * options.scale) + border_x 
			- (options.scale * chars->start_at_x));
		int yy((y * font->char_h * options.scale) + border_y
			- (options.scale * chars->start_at_y));
		CharAttr attr(ch.cursor ? (CharAttr) { 1 } : ch.attr);
		SDL_Surface* sf(chars->Char(ch.ch, attr, x+y));
		if(!sf)
			abort();
		rects[i] = (SDL_Rect) { xx, yy, sf->w, sf->h };

		// clear char
		SDL_Rect r2 = { xx+(options.scale * chars->start_at_y),
			yy+(options.scale * chars->start_at_y),
			font->char_w * options.scale,
			font->char_h * options.scale+1
		};
		if(x == 0) { r2.x-=2; r2.w+=2; } else if(x == (w-1)) { r2.w+=2; }
		if(y == 0) { r2.y-=2; r2.h+=2; } else if(y == (h-1)) { r2.h+=2; }
		SDL_FillRect(screen, &r2, 0);

		// draw char
		if(!((ch.attr.Blink || ch.cursor) && !terminal.BlinkOn()))
			SDL_BlitSurface(sf, NULL, screen, &rects[i]);
		else if(ch.cursor && !terminal.BlinkOn())
		{
			// show underletter when blinking is off
			SDL_Surface* sf2(chars->Char(ch.ch, ch.attr, x+y));
			SDL_BlitSurface(sf2, NULL, screen, &rects[i]);
		}
		i++;
	}
	terminal.dirty.clear();

	// update screen
	SDL_UpdateRects(screen, i, rects);
}
