#ifndef SCREEN_H
#define SCREEN_H

#include <vector>
#include <deque>
using namespace std;

#include "global.h"

#include "SDL2/SDL.h"

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

private:
	void KeyEvent(SDL_KeyboardEvent key) const;

	SDL_Window* win;
	SDL_Renderer* ren;
};

#endif
