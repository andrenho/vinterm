#include "graphic/screen.h"

#include <iostream>

#include "terminal/keyqueue.h"
#include "render/renderer.h"

Screen::Screen(Options const& options, Renderer const& renderer, Mouse& mouse)
	: options(options), renderer(renderer), mouse(mouse),
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
			640, 480, // TODO - calculate font size
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

	// prepare to resize
	int w, h;
	// SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
	// SDL_SetWindowFullscreen(win, 0);
	SDL_GetWindowSize(win, &w, &h);
	keyQueue.push_back(RESIZE);
	keyQueue.push_back(w);
	keyQueue.push_back(h);
	keyQueue.push_back(0);

	// clear screen
	SDL_RenderClear(ren);
	SDL_RenderPresent(ren);
}


Screen::~Screen()
{
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}


void
Screen::Resize(int new_w, int new_h, int full_screen, int& ts_w, int& ts_h)
{
	renderer.Resize(new_w, new_h, ts_w, ts_h);
}


void
Screen::Update()
{
	renderer.Render();
}


void
Screen::CheckEvents()
{
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
		case SDL_QUIT:
			keyQueue.push_back(QUIT);
			break;
		}
	}
}
