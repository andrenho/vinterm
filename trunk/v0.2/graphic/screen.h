#ifndef SCREEN_H
#define SCREEN_H

#include "SDL.h"

#include "options.h"
#include "terminal.h"
#include "chars.h"

#define P(sf,x,y) *((Uint8*)(sf)->pixels + (y)*(sf)->pitch + (x))

class Screen
{
public:
	Screen(Options const& options, Terminal& terminal);
	~Screen();

	void Update();

	SDL_Surface* ScreenSurface() const { return screen; }
	static void initializePalette(SDL_Surface* sf, Options const& options);

private:
	void initializeChars();

	Options const& options;
	const Font* const font;
	Chars* const chars;
	Terminal const& terminal;
	const int border_x, border_y;
	const int w, h;
	SDL_Surface* screen;
	SDL_Rect* const rects;
};

#endif
