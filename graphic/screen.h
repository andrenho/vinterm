#ifndef SCREEN_H
#define SCREEN_H

#include "SDL.h"

#include <vector>
#include <deque>
using namespace std;

class Options;
class Framebuffer;
class Chars;
class Font;

#define P(sf,x,y) *((Uint8*)(sf)->pixels + (y)*(sf)->pitch + (x))

enum { 
	F1=256, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	K_UP, K_DOWN, K_LEFT, K_RIGHT, HOME, DELETE, PAGE_UP, PAGE_DOWN,
	INSERT, END, SH_UP, SH_DOWN, SH_LEFT, SH_RIGHT, CT_PAGE_UP,
	CT_PAGE_DOWN, QUIT
};


class Screen
{
public:
	Screen(Options const& options, Framebuffer const& fb);
	~Screen();

	void Update();

	SDL_Surface* ScreenSurface() const { return screen; }
	static void initializePalette(SDL_Surface* sf, Options const& options);
	deque<int> keyQueue;

private:
	void initializeChars();
	void CheckForBlink();
	void DoEvents();
	void Resize(int new_w, int new_h);

	Options const& options;
	const Font* const font;
	Chars* const chars;
	Framebuffer const& fb;
	const int border_x, border_y;
	const int w, h;
	SDL_Surface* screen;
	int old_cursor_x, old_cursor_y;
};

#endif
