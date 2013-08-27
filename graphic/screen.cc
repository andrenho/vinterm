#include "graphic/screen.h"

#include <iostream>

#include "terminal/charmatrix.h"
#include "terminal/keyqueue.h"
#include "render/renderer.h"

Screen::Screen()
	: win(nullptr), ren(nullptr)
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
	renderer->Resize(new_w, new_h, ts_w, ts_h);
}


void
Screen::Update()
{
	// render image
	renderer->Render();

	// set window title
	string new_title = cm->TerminalTitle();
	const char* title = SDL_GetWindowTitle(win);
	if(strcmp(title, new_title.c_str()) != 0)
		SDL_SetWindowTitle(win, new_title.c_str());
}


void
Screen::CheckEvents() const
{
	int i = 0;
	int x, y;
	uint8_t state;

	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
		case SDL_KEYDOWN:
			KeyEvent(e.key);
			break;

		case SDL_TEXTINPUT:
			i = 0;
			while(e.text.text[i])
				keyQueue.push_back(e.text.text[i++]);
			break;

		/*
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			SDL_ShowCursor(SDL_ENABLE);
			if(!mouse->Captured() && e.button.button == 2  // paste
					&& e.type == SDL_MOUSEBUTTONDOWN) 
			{
				for(char c : fb.clipboard.Read())
					keyQueue.push_back(c);
			}
			else   // send mouse event to application
			{
				renderer->CharPosition(e.button.x, e.button.y, x, y);
				if(x >= 0 && y >= 0)
				{
					SDLMod mod = SDL_GetModState();
					mouse->AddButtonPressToQueue(
							e.type == SDL_MOUSEBUTTONDOWN,
							x, y, e.button.button,
							mod & KMOD_SHIFT,
							mod & KMOD_ALT,
							mod & KMOD_CTRL, false);
				}
			}
			break;

		case SDL_MOUSEMOTION:
			SDL_ShowCursor(SDL_ENABLE);
			state = SDL_GetMouseState(NULL, NULL);
			if(state & (SDL_BUTTON(1) | SDL_BUTTON(2) | SDL_BUTTON(3)))
			{
				renderer->CharPosition(e.motion.x, e.motion.y, x, y);
				mouse->Drag(x, y, state);
			}
			break; */

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
	static struct { char c; uint8_t code; } ctrl_codes[] = {
		{ 'a', 1 }, { 'b', 2 }, { 'c', 3 }, { 'd', 4 }, { 'e', 5 },
		{ 'f', 6 }, { 'g', 7 }, { 'h', 8 }, { 'i', 9 }, { 'j', 10 },
		{ 'k', 11 }, { 'l', 12 }, { 'm', 13 }, { 'n', 14 }, { 'o', 15 },
		{ 'p', 16 }, { 'q', 17 }, { 'r', 18 }, { 's', 19 }, { 't', 20 },
		{ 'u', 21 }, { 'v', 22 }, { 'w', 23 }, { 'x', 24 }, { 'y', 25 },
		{ 'z', 26 }, { '[', 27 }, { '\\', 28 }, { ']', 29 }, { '^', 30 },
		{ '_', 31 },
	};

	SDL_ShowCursor(SDL_DISABLE);

	// check for control codes
	if(key.keysym.mod & KMOD_CTRL)
		for(auto cc : ctrl_codes)
			if(cc.c == key.keysym.sym)
			{
				keyQueue.push_back(cc.code);
				return;
			}

	// check for other codes
	switch(key.keysym.sym)
	{
	case SDLK_RETURN:
		keyQueue.push_back('\n'); break;
	case SDLK_BACKSPACE:
		keyQueue.push_back('\b'); break;
	case SDLK_TAB:
		keyQueue.push_back('\t'); break;
	case SDLK_ESCAPE:
		keyQueue.push_back(27); break;
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
		if(key.keysym.mod & KMOD_CTRL)
		{
			// full screen
			int w, h;
			SDL_GetWindowSize(win, &w, &h);
			keyQueue.push_back(RESIZE);
			keyQueue.push_back(w);
			keyQueue.push_back(h);
			keyQueue.push_back((key.keysym.mod & KMOD_SHIFT) + 1);
		}
		else
			keyQueue.push_back(F11); 
		break;
	case SDLK_F12:
		keyQueue.push_back(F12); break;
	case SDLK_UP:
		if(key.keysym.mod & KMOD_SHIFT)
			keyQueue.push_back(SH_UP);
		else
			keyQueue.push_back(K_UP);
		break;
	case SDLK_DOWN:
		if(key.keysym.mod & KMOD_SHIFT)
			keyQueue.push_back(SH_DOWN);
		else
			keyQueue.push_back(K_DOWN);
		break;
	case SDLK_LEFT:
		if(key.keysym.mod & KMOD_SHIFT)
			keyQueue.push_back(SH_LEFT);
		else
			keyQueue.push_back(K_LEFT);
		break;
	case SDLK_RIGHT:
		if(key.keysym.mod & KMOD_SHIFT)
			keyQueue.push_back(SH_RIGHT);
		else
			keyQueue.push_back(K_RIGHT);
		break;
	case SDLK_HOME:
		keyQueue.push_back(HOME); break;
	case SDLK_DELETE:
		keyQueue.push_back(DELETE); break;
	case SDLK_PAGEUP:
		if(key.keysym.mod & KMOD_CTRL)
			keyQueue.push_back(CT_PAGE_UP);
		else if(key.keysym.mod & KMOD_SHIFT)
			keyQueue.push_back(SH_PAGE_UP);
		else
			keyQueue.push_back(PAGE_UP);
		break;
	case SDLK_PAGEDOWN:
		if(key.keysym.mod & KMOD_CTRL)
			keyQueue.push_back(CT_PAGE_DOWN);
		else if(key.keysym.mod & KMOD_SHIFT)
			keyQueue.push_back(SH_PAGE_DOWN);
		else
			keyQueue.push_back(PAGE_DOWN);
		break;
	case SDLK_INSERT:
		keyQueue.push_back(INSERT); break;
	case SDLK_END:
		keyQueue.push_back(END); break;
	}
}
