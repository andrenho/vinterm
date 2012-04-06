#include "graphic/screen.h"

#include <cstdlib>
#include <set>
#include "SDL.h"
using namespace std;

#include "options.h"
#include "terminal/framebuffer.h"
#include "graphic/blink.h"
#include "graphic/chars.h"
#include "graphic/font.h"

Screen::Screen(Options const& options, Framebuffer const& fb)
	: options(options), font(new Font()), 
	  chars(new Chars(options, *font)), fb(fb),
	  border_x(options.border_x * options.scale),
	  border_y(options.border_y * options.scale),
	  w(fb.W() * font->char_w * options.scale + (border_x * 2)), 
	  h(fb.H() * font->char_h * options.scale + (border_y * 2)),
	  blink(new Blink(options.blink_speed)), 
	  old_cursor_x(0), old_cursor_y(0)
{
	SDL_Init(SDL_INIT_VIDEO);

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
	SDL_Quit();
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

	// check if cursor changed position, and register the change
	CheckForCursor();

	// update data from the framebuffer
	set<int>::const_iterator n;
	for(n = fb.dirty->begin(); n != fb.dirty->end(); n++)
	{
		// get framebuffer positions
		int x = (*n) % fb.W();
		int y = (*n) / fb.W();

		// verify if it's the cursor
		bool cursor = (x == fb.CursorX() && y == fb.CursorY());

		// get character to draw and adjust attribute
		Char ch = fb.Ch(x, y);
		char c = ch.Ch;
		Attribute attr = ch.Attr;
		if(attr.Blink || cursor)
		{
			if(blink->State())
				attr.Reverse = !attr.Reverse;
			else if(!cursor)
				c = ' ';
		}

		SDL_Surface* sf = chars->Char(c, attr, x+y);
		if(!sf)
			abort();

		// find position on the screen
		int xx = (x * font->char_w * options.scale) + border_x 
			- (options.scale * chars->start_at_x);
		int yy = (y * font->char_h * options.scale) + border_y
			- (options.scale * chars->start_at_y);
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
		SDL_BlitSurface(sf, NULL, screen, &r);

		// register char location for later update
		rects.push_back(r);
	}
	fb.dirty->clear();

	// update screen
	SDL_UpdateRects(screen, rects.size(), &rects[0]);

	// gives the OS some rest
	SDL_Delay(5);
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
			{
				blink->ResetClock();
				return c;
			}
			break;
		case SDL_QUIT:
			return QUIT;
		}
	}
	return 0;
}


void
Screen::CheckForCursor()
{
	// blink, if necessary
	if(blink->TimeToBlink())
	{
		blink->DoBlink(fb);
		fb.dirty->insert(fb.CursorX() + fb.CursorY() * fb.W());
	}

	// register a change where the cursor was and where it is now
	if(fb.CursorX() != old_cursor_x || fb.CursorY() != old_cursor_y)
	{
		fb.dirty->insert(old_cursor_x + old_cursor_y * fb.W());
		fb.dirty->insert(fb.CursorX() + fb.CursorY() * fb.W());
		old_cursor_x = fb.CursorX();
		old_cursor_y = fb.CursorY();
	}
}
