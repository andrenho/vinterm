#include "input.h"

#include "SDL.h"
#include "terminal/blink.h"

int 
Event(Blink* blink)
{
	// read chars and send them to the console
	SDL_Event e;
	uint16_t c;

	while(SDL_PollEvent(&e))
	{
		Uint8* k = SDL_GetKeyState(NULL);
		switch(e.type)
		{
		case SDL_KEYDOWN:
			switch(e.key.keysym.sym)
			{
			case SDLK_F1:
				return F1;
			case SDLK_F2:
				return F2;
			case SDLK_F3:
				return F3;
			case SDLK_F4:
				return F4;
			case SDLK_F5:
				return F5;
			case SDLK_F6:
				return F6;
			case SDLK_F7:
				return F7;
			case SDLK_F8:
				return F8;
			case SDLK_F9:
				return F9;
			case SDLK_F10:
				return F10;
			case SDLK_F11:
				return F11;
			case SDLK_F12:
				return F12;
			case SDLK_UP:
				if(k[SDLK_RSHIFT] || k[SDLK_LSHIFT])
					return SH_UP;
				else
					return K_UP;
			case SDLK_DOWN:
				if(k[SDLK_RSHIFT] || k[SDLK_LSHIFT])
					return SH_DOWN;
				else
					return K_DOWN;
			case SDLK_LEFT:
				if(k[SDLK_RSHIFT] || k[SDLK_LSHIFT])
					return SH_LEFT;
				else
					return K_LEFT;
			case SDLK_RIGHT:
				if(k[SDLK_RSHIFT] || k[SDLK_LSHIFT])
					return SH_RIGHT;
				else
					return K_RIGHT;
			case SDLK_HOME:
				return HOME;
			case SDLK_DELETE:
				return DELETE;
			case SDLK_PAGEUP:
				if(k[SDLK_LCTRL] || k[SDLK_RCTRL])
					return CT_PAGE_UP;
				else
					return PAGE_UP;
			case SDLK_PAGEDOWN:
				if(k[SDLK_LCTRL] || k[SDLK_RCTRL])
					return CT_PAGE_DOWN;
				else
					return PAGE_DOWN;
			case SDLK_INSERT:
				return INSERT;
			case SDLK_END:
				return END;
			default:
				c = e.key.keysym.unicode;
				if(c != 0)
				{
					blink->ResetClock();
					return c;
				}
				else
					return 0;
			}
			break;
		case SDL_QUIT:
			return QUIT;
		}
	}
	return 0;
}
