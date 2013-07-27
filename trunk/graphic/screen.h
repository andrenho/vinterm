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

#define P(sf,x,y) *((Uint8*)(sf)->pixels + (y)*(sf)->pitch + (x))

enum { 
	F1=256, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	K_UP, K_DOWN, K_LEFT, K_RIGHT, HOME, DELETE, PAGE_UP, PAGE_DOWN,
	INSERT, END, SH_UP, SH_DOWN, SH_LEFT, SH_RIGHT, CT_PAGE_UP,
	CT_PAGE_DOWN, SH_PAGE_UP, SH_PAGE_DOWN, QUIT, RESIZE
};


typedef struct FullScreenInfo {
	bool isFullScreen;
	int old_w, old_h, old_border_x;
	
	FullScreenInfo() : isFullScreen(false), old_w(0), old_h(0) { }
} FullScreenInfo;


class Screen
{
public:
	Screen(Options const& options, Framebuffer const& fb);
	~Screen();

	void Update();
	void Resize(int new_w, int new_h, int full_screen, 
			int& ts_w, int& ts_h);

	SDL_Surface* ScreenSurface() const { return screen; }
	static void initializePalette(SDL_Surface* sf, Options const& options);

	string const& FontEncoding() const { return font->Encoding; }

	deque<uint32_t> keyQueue;
	static SDL_Color palette[256];
	Audio audio;

private:
	void initializeChars();
	void CheckForBlink();
	void DoEvents();

	Options const& options;
	const Font* const font;
	Chars* const chars;
	Framebuffer const& fb;
	int border_x, border_y;
	int w, h;
	SDL_Surface* screen;
	int old_cursor_x, old_cursor_y;
	int desktop_w, desktop_h;
	FullScreenInfo fs_info;
	SDL_Surface* icon;
};

#endif
