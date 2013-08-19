#include "graphic/screen.h"

#include <iostream>

Screen::Screen(Options const& options, Framebuffer const& fb, Mouse& mouse)
	: options(options), fb(fb), mouse(mouse),
	  win(nullptr), ren(nullptr)
{
	// initialize SDL
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		cerr << "error: could not initialize SDL2" << endl;
		throw SDL_GetError();
	}

	// create window
	win = SDL_CreateWindow("vinterm " VERSION,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			400, 400, // TODO - calculate font size
			SDL_WINDOW_SHOWN);
	if(win == nullptr)
	{
		cerr << "error: could not initialize SDL2 window" << endl;
		throw SDL_GetError();
	}

	// create renderer
	ren = SDL_CreateRenderer(win, -1, 
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(ren == nullptr)
	{
		cerr << "error: could not initialize SDL2 renderer" << endl;
		throw SDL_GetError();
	}
	SDL_RenderClear(ren);
	SDL_RenderPresent(ren);
}


void
Screen::Resize(int new_w, int new_h, int full_screen, int& ts_w, int& ts_h)
{
}


void
Screen::Update()
{
}


Screen::~Screen()
{
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}
