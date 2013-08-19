#ifndef SCREEN_H
#define SCREEN_H

#include "SDL.h"

#include <vector>
#include <deque>
using namespace std;

#include "graphic/audio.h"
#include "graphic/font.h"

class Options;
class Framebuffer;
class Chars;
class Font;
class Mouse;

#define P(sf,x,y) *((Uint8*)(sf)->pixels + (y)*(sf)->pitch + (x))

typedef struct FullScreenInfo {
	bool isFullScreen;
	int old_w, old_h, old_border_x;
	
	FullScreenInfo() : isFullScreen(false), old_w(0), old_h(0) { }
} FullScreenInfo;

class Screen
{
public:
	Screen(Options const& options, Framebuffer const& fb, Mouse& mouse);
	~Screen();

	void Update();
	void Resize(int new_w, int new_h, int full_screen, 
			int& ts_w, int& ts_h);

	SDL_Surface* ScreenSurface() const { return screen; }
	static void initializePalette(SDL_Surface* sf, Options const& options);

	string const& FontEncoding() const { return font->Encoding; }

	static SDL_Color palette[256];

private:
	void initializeChars();
	void CheckForBlink();
	void DoEvents();
	void CharPosition(int mx, int my, int& x, int& y) const;

	Options const& options;
	Mouse& mouse;
	const Font* const font;
	Chars* const chars;
	Framebuffer const& fb;
	int border_x, border_y;
	int w, h;
	SDL_Surface* screen;
	int old_cursor_x, old_cursor_y; // used for blink
	int desktop_w, desktop_h;
	FullScreenInfo fs_info;
	SDL_Surface* icon;
};

#endif
