#ifndef SCREEN_H
#define SCREEN_H

#include <vector>
#include <deque>
using namespace std;

#include "graphic/audio.h"

#include "SDL2/SDL.h"

class Options;
class Framebuffer;
class Renderer;
class Mouse;

class Screen
{
public:
	Screen(Options const& options, Framebuffer& fb, Renderer const& renderer, 
			Mouse& mouse);
	~Screen();

	void Update();
	void Resize(int new_w, int new_h, int full_screen, 
			int& ts_w, int& ts_h);
	void CheckEvents();

private:
	Options const& options;
	Framebuffer& fb;
	Renderer const& renderer;
	Mouse& mouse;

	SDL_Window* win;
	SDL_Renderer* ren;
};

#endif
