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

	// receive UNICODE input
	SDL_StartTextInput();
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
Screen::CheckEvents() const
{
	uint16_t c;
	int i = 0;

	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
		/* case SDL_KEYDOWN:
			KeyEvent(e.key);
			break; */

		case SDL_TEXTINPUT:
			i = 0;
			c = 0;
			while(e.text.text[i])
			{
				printf("%x\n", e.text.text[i] & 0xff);
				keyQueue.push_back(e.text.text[i] & 0xff);
				c <<= 8;
				c += (e.text.text[i++] & 0xff);
			}
			printf("%d\n", c);
			//keyQueue.push_back(c);
			break;

		case SDL_WINDOWEVENT:
			if(e.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				keyQueue.push_back(RESIZE);
				keyQueue.push_back(e.window.data1);
				keyQueue.push_back(e.window.data2);
				keyQueue.push_back(0);
			}
			break;

		case SDL_QUIT:
			keyQueue.push_back(QUIT);
			break;
		}
	}
}


void
Screen::KeyEvent(SDL_KeyboardEvent key) const
{
	uint16_t c;
	//Uint8* k = SDL_GetKeyState(NULL);

	SDL_ShowCursor(SDL_DISABLE);
//	cm.blink->ResetClock();

	switch(key.keysym.sym)
	{
		/*
	case SDLK_F1:
		keyQueue.push_back(F1); break;
	case SDLK_F2:
		keyQueue.push_back(F2); break;
	case SDLK_F3:
		keyQueue.push_back(F3); break;
	case SDLK_F4:
		keyQueue.push_back(F4); break;
	case SDLK_F5:
		keyQueue.push_back(F5); break;
	case SDLK_F6:
		keyQueue.push_back(F6); break;
	case SDLK_F7:
		keyQueue.push_back(F7); break;
	case SDLK_F8:
		keyQueue.push_back(F8); break;
	case SDLK_F9:
		keyQueue.push_back(F9); break;
	case SDLK_F10:
		keyQueue.push_back(F10); break;
	case SDLK_F11:
		if(k[SDLK_RCTRL] || k[SDLK_LCTRL])
		{
			// full screen
			keyQueue.push_back(RESIZE);
			keyQueue.push_back(desktop_w);
			keyQueue.push_back(desktop_h);
			keyQueue.push_back((k[SDLK_RSHIFT] || k[SDLK_LSHIFT]) + 1);
		}
		else
			keyQueue.push_back(F11); 
		break;
	case SDLK_F12:
		keyQueue.push_back(F12); break;
	case SDLK_UP:
		if(k[SDLK_RSHIFT] || k[SDLK_LSHIFT])
			keyQueue.push_back(SH_UP);
		else
			keyQueue.push_back(K_UP);
		break;
	case SDLK_DOWN:
		if(k[SDLK_RSHIFT] || k[SDLK_LSHIFT])
			keyQueue.push_back(SH_DOWN);
		else
			keyQueue.push_back(K_DOWN);
		break;
	case SDLK_LEFT:
		if(k[SDLK_RSHIFT] || k[SDLK_LSHIFT])
			keyQueue.push_back(SH_LEFT);
		else
			keyQueue.push_back(K_LEFT);
		break;
	case SDLK_RIGHT:
		if(k[SDLK_RSHIFT] || k[SDLK_LSHIFT])
			keyQueue.push_back(SH_RIGHT);
		else
			keyQueue.push_back(K_RIGHT);
		break;
	case SDLK_HOME:
		keyQueue.push_back(HOME); break;
	case SDLK_DELETE:
		keyQueue.push_back(DELETE); break;
	case SDLK_PAGEUP:
		if(k[SDLK_LCTRL] || k[SDLK_RCTRL])
			keyQueue.push_back(CT_PAGE_UP);
		else if(k[SDLK_LSHIFT] || k[SDLK_RSHIFT])
			keyQueue.push_back(SH_PAGE_UP);
		else
			keyQueue.push_back(PAGE_UP);
		break;
	case SDLK_PAGEDOWN:
		if(k[SDLK_LCTRL] || k[SDLK_RCTRL])
			keyQueue.push_back(CT_PAGE_DOWN);
		else if(k[SDLK_LSHIFT] || k[SDLK_RSHIFT])
			keyQueue.push_back(SH_PAGE_DOWN);
		else
			keyQueue.push_back(PAGE_DOWN);
		break;
	case SDLK_INSERT:
		keyQueue.push_back(INSERT); break;
	case SDLK_END:
		keyQueue.push_back(END); break;
		*/
	default:
		c = key.keysym.sym; //unicode;
		if(c != 0)
			keyQueue.push_back(c);
	}
}
