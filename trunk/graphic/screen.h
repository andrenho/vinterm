#ifndef SCREEN_H
#define SCREEN_H

#include <vector>
#include <deque>
using namespace std;

#include "global.h"

#include "SDL2/SDL.h"

#define FPS 50

class Screen
{
public:
	Screen();
	~Screen();

	void Update();
	void Resize(int new_w, int new_h, int full_screen, 
			int& ts_w, int& ts_h);
	void CheckEvents() const;
	SDL_Renderer* GLRenderer() const { return ren; }
	void WaitNextFrame();

	SDL_Window* SDLWindow() const { return win; } 

private:
	void KeyEvent(SDL_KeyboardEvent key) const;

	SDL_Window* win;
	SDL_Renderer* ren;

	uint32_t last_frame;
	bool full_screen;
};

#endif
