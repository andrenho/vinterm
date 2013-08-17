#include "graphic/screen.h"

#include <cstdlib>
#include <iostream>
#include <set>
#include "SDL.h"
using namespace std;

#include "options.h"
#include "terminal/blink.h"
#include "terminal/framebuffer.h"
#include "terminal/mouse.h"
#include "graphic/chars.h"
#include "graphic/font.h"

SDL_Color Screen::palette[256];

Screen::Screen(Options const& options, Framebuffer const& fb, Mouse& mouse)
	: options(options), mouse(mouse), font(new Font()), 
	  chars(new Chars(options, *font)), fb(fb),
	  border_x(options.border_x * options.scale),
	  border_y(options.border_y * options.scale),
	  w(fb.W() * font->char_w * options.scale + (border_x * 2)), 
	  h(fb.H() * font->char_h * options.scale + (border_y * 2)),
	  old_cursor_x(0), old_cursor_y(0), icon(nullptr)
{
	SDL_Init(SDL_INIT_VIDEO);

	// check window size
	SDL_VideoInfo const* vi = SDL_GetVideoInfo();
	desktop_w = vi->current_w;
	desktop_h = vi->current_h;

	// load icon
	icon = SDL_LoadBMP(DATADIR "/icon_16.bmp");
	if(icon)
	{
		Uint32 colorkey = SDL_MapRGB(icon->format, 255, 0, 255);
		SDL_SetColorKey(icon, SDL_SRCCOLORKEY, colorkey);
		SDL_WM_SetIcon(icon, NULL);
	}

	// create icon
	screen = SDL_SetVideoMode(w, h, 8, SDL_SWSURFACE|SDL_RESIZABLE);
	if(!screen)
	{
		fprintf(stderr, "It was not possible to open the display: %s\n",
				SDL_GetError());
		exit(1);
	}
	SDL_WM_SetCaption("Vintage Emulator " VERSION, "Vintage Emulator");

	// setup keyboard
	SDL_EnableUNICODE(1);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, 
			SDL_DEFAULT_REPEAT_INTERVAL);

	// setup palette
	initializePalette(screen, options);

	// schedule resize
	keyQueue.push_back(RESIZE);
	keyQueue.push_back(w);
	keyQueue.push_back(h);
	keyQueue.push_back(0);

	// hide cursor
	SDL_ShowCursor(SDL_DISABLE);
}


Screen::~Screen()
{
	if(icon)
		SDL_FreeSurface(icon);
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
	CheckForBlink();

	// update data from the framebuffer
	set<int>::const_iterator n;
	for(n = fb.dirty->begin(); n != fb.dirty->end(); n++)
	{
		// get framebuffer positions
		int x = (*n) % fb.W();
		int y = (*n) / fb.W();

		// verify if it's the cursor
		bool cursor = (x == fb.CursorX() && y == fb.CursorY());
		if(fb.CursorVisibility == NOT_VISIBLE)
			cursor = false;

		// get character to draw and adjust attribute
		Char ch = fb.Ch(x, y);
		char c = ch.Ch;
		Attribute attr = ch.Attr;
		if(attr.Blink || cursor)
		{
			if(fb.blink->State())
				attr.Reverse = !attr.Reverse;
			else if(!cursor)
				c = ' ';
		}
		else if(attr.Invisible)
			c = ' ';
		if(cursor && fb.CursorVisibility == VERY_VISIBLE)
			attr.Highlight = true;
		if(fb.IsSelected(x, y))
			attr.Reverse = !attr.Reverse;

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
			(Uint16)sf->w, (Uint16)sf->h 
		};

		// clear char
		SDL_Rect r2 = { 
			(Sint16)(xx+(options.scale * chars->start_at_y)),
			(Sint16)(yy+(options.scale * chars->start_at_y)),
			(Uint16)(font->char_w * options.scale),
			(Uint16)(font->char_h * options.scale+1)
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

	// check for events
	DoEvents();

	// gives the OS some rest
	SDL_Delay(5);

	// holds for flashing
	if(fb.Flashing())
		SDL_Delay(options.flashing_speed);

	// respond to clipboard requests
	fb.clipboard.Respond();

	// check the window title
	char *title, *icon;
	SDL_WM_GetCaption(&title, &icon);
	if(strcmp(title, fb.terminalTitle().c_str()) != 0)
		SDL_WM_SetCaption(fb.terminalTitle().c_str(),
				  fb.terminalTitle().c_str());
}


void
Screen::CheckForBlink()
{
	// blink, if necessary
	if(fb.blink->TimeToBlink())
	{
		fb.blink->DoBlink(fb);
		fb.dirty->insert(fb.CursorX() + fb.CursorY() * fb.W());
		fb.RegisterBlinks();
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


void
Screen::DoEvents()
{
	SDL_Event e;
	uint16_t c;
	int x, y;
	uint8_t state;

	while(SDL_PollEvent(&e))
	{
		Uint8* k = SDL_GetKeyState(NULL);
		switch(e.type)
		{
		case SDL_KEYDOWN:
			SDL_ShowCursor(SDL_DISABLE);
			fb.blink->ResetClock();
			switch(e.key.keysym.sym)
			{
			case SDLK_F1:
				keyQueue.push_back(F1); break;
			case SDLK_F2:
				keyQueue.push_back(F2); break;
			case SDLK_F3:
				keyQueue.push_back(F3); break;
			case SDLK_F4:
				keyQueue.push_back(F4); break;
			case SDLK_F5:
				keyQueue.push_back(F5); break;
			case SDLK_F6:
				keyQueue.push_back(F6); break;
			case SDLK_F7:
				keyQueue.push_back(F7); break;
			case SDLK_F8:
				keyQueue.push_back(F8); break;
			case SDLK_F9:
				keyQueue.push_back(F9); break;
			case SDLK_F10:
				keyQueue.push_back(F10); break;
			case SDLK_F11:
				if(k[SDLK_RCTRL] || k[SDLK_LCTRL])
				{
					// full screen
					keyQueue.push_back(RESIZE);
					keyQueue.push_back(desktop_w);
					keyQueue.push_back(desktop_h);
					keyQueue.push_back((k[SDLK_RSHIFT] || k[SDLK_LSHIFT]) + 1);
				}
				else
					keyQueue.push_back(F11); 
				break;
			case SDLK_F12:
				keyQueue.push_back(F12); break;
			case SDLK_UP:
				if(k[SDLK_RSHIFT] || k[SDLK_LSHIFT])
					keyQueue.push_back(SH_UP);
				else
					keyQueue.push_back(K_UP);
				break;
			case SDLK_DOWN:
				if(k[SDLK_RSHIFT] || k[SDLK_LSHIFT])
					keyQueue.push_back(SH_DOWN);
				else
					keyQueue.push_back(K_DOWN);
				break;
			case SDLK_LEFT:
				if(k[SDLK_RSHIFT] || k[SDLK_LSHIFT])
					keyQueue.push_back(SH_LEFT);
				else
					keyQueue.push_back(K_LEFT);
				break;
			case SDLK_RIGHT:
				if(k[SDLK_RSHIFT] || k[SDLK_LSHIFT])
					keyQueue.push_back(SH_RIGHT);
				else
					keyQueue.push_back(K_RIGHT);
				break;
			case SDLK_HOME:
				keyQueue.push_back(HOME); break;
			case SDLK_DELETE:
				keyQueue.push_back(DELETE); break;
			case SDLK_PAGEUP:
				if(k[SDLK_LCTRL] || k[SDLK_RCTRL])
					keyQueue.push_back(CT_PAGE_UP);
				else if(k[SDLK_LSHIFT] || k[SDLK_RSHIFT])
					keyQueue.push_back(SH_PAGE_UP);
				else
					keyQueue.push_back(PAGE_UP);
				break;
			case SDLK_PAGEDOWN:
				if(k[SDLK_LCTRL] || k[SDLK_RCTRL])
					keyQueue.push_back(CT_PAGE_DOWN);
				else if(k[SDLK_LSHIFT] || k[SDLK_RSHIFT])
					keyQueue.push_back(SH_PAGE_DOWN);
				else
					keyQueue.push_back(PAGE_DOWN);
				break;
			case SDLK_INSERT:
				keyQueue.push_back(INSERT); break;
			case SDLK_END:
				keyQueue.push_back(END); break;
			default:
				c = e.key.keysym.unicode;
				if(c != 0)
					keyQueue.push_back(c);
			}
			break;

		case SDL_VIDEORESIZE:
			keyQueue.push_back(RESIZE);
			keyQueue.push_back(e.resize.w);
			keyQueue.push_back(e.resize.h);
			keyQueue.push_back(0);
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			SDL_ShowCursor(SDL_ENABLE);
			if(!mouse.Captured() && e.button.button == 2  // paste
					&& e.type == SDL_MOUSEBUTTONDOWN) 
			{
				for(char c : fb.clipboard.Read())
					keyQueue.push_back(c);
			}
			else   // send mouse event to application
			{
				CharPosition(e.button.x, e.button.y, x, y);
				if(x >= 0 && y >= 0)
				{
					SDLMod mod = SDL_GetModState();
					mouse.AddButtonPressToQueue(keyQueue,
							e.type == SDL_MOUSEBUTTONDOWN,
							x, y, e.button.button,
							mod & KMOD_SHIFT,
							mod & KMOD_ALT,
							mod & KMOD_CTRL, false);
				}
			}
			break;

		case SDL_MOUSEMOTION:
			SDL_ShowCursor(SDL_ENABLE);
			state = SDL_GetMouseState(NULL, NULL);
			if(state & (SDL_BUTTON(1) | SDL_BUTTON(2) | SDL_BUTTON(3)))
			{
				CharPosition(e.motion.x, e.motion.y, x, y);
				mouse.Drag(keyQueue, x, y, state);
			}
			break;

		case SDL_QUIT:
			keyQueue.push_back(QUIT);
			break;
		}
	}
}


void
Screen::Resize(int new_w, int new_h, int full_screen, int& ts_w, int& ts_h)
{
	// if returning from full screen, restore previous size
	if(full_screen && fs_info.isFullScreen)
	{
		new_w = fs_info.old_w;
		new_h = fs_info.old_h;
		border_x = fs_info.old_border_x;
	}

	// if fullscreen with 80 columns, get the correct border width
	if(full_screen == 2 && !fs_info.isFullScreen)
	{
		fs_info.old_border_x = border_x;
		border_x = (desktop_w - (font->char_w * options.scale * 80)) / 2;
	}

	// detect new terminal size
	ts_w = (new_w - (border_x * 2)) / options.scale / font->char_w;
	ts_h = (new_h - (border_y * 2)) / options.scale / font->char_h;
	if(ts_w < 1)
		ts_w = 1;
	if(ts_h < 1)
		ts_h = 1;

	// set flags
	Uint32 flags = SDL_SWSURFACE | SDL_RESIZABLE;
	if(full_screen && !fs_info.isFullScreen)
	{
		flags |= SDL_FULLSCREEN;
		fs_info.old_w = screen->w;
		fs_info.old_h = screen->h;
		if(full_screen == 1)
			fs_info.old_border_x = border_x;
	}

	// create new screen
	screen = SDL_SetVideoMode(new_w, new_h, 8, flags);
	if(!screen)
	{
		fprintf(stderr, "It was not possible to open the display: %s\n",
				SDL_GetError());
		exit(1);
	}
	SDL_SetColors(screen, palette, 0, 256);

	// adjust fullscreen information
	if(full_screen)
		fs_info.isFullScreen = !fs_info.isFullScreen;

	// show/hide mouse
	SDL_ShowCursor(fs_info.isFullScreen ? SDL_DISABLE : SDL_ENABLE);
	
	// store new size
	w = new_w;
	h = new_h;
}


void 
Screen::CharPosition(int mx, int my, int& x, int& y) const
{
	x = (mx - border_x + (options.scale * chars->start_at_x)) / font->char_w / options.scale;
	if(x >= fb.W())
		x = -1;
	y = (my - border_y + (options.scale * chars->start_at_y)) / font->char_h / options.scale;
	if(y >= fb.H())
		y = -1;
}
