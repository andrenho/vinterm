#ifndef SCREEN_H
#define SCREEN_H

#include "SDL.h"

#include <vector>
using namespace std;

class Options;
class Framebuffer;
class Chars;
class Font;
class Blink;

#define P(sf,x,y) *((Uint8*)(sf)->pixels + (y)*(sf)->pitch + (x))

class Screen
{
public:
	Screen(Options const& options, Framebuffer const& fb);
	~Screen();

	void Update();
	int Input() const;

	SDL_Surface* ScreenSurface() const { return screen; }
	static void initializePalette(SDL_Surface* sf, Options const& options);

private:
	void initializeChars();
	void CheckForBlink();

	Options const& options;
	const Font* const font;
	Chars* const chars;
	Framebuffer const& fb;
	const int border_x, border_y;
	const int w, h;
	SDL_Surface* screen;
	Blink* blink;
	int old_cursor_x, old_cursor_y;
};

typedef enum { QUIT=256 } Keys;

#endif
