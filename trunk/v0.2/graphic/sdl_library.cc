#include "graphic/sdl_library.h"

#include <string>
using namespace std;

#include "SDL.h"

SDL_Library::SDL_Library(int const w, int const h) 
{
	if(SDL_Init(SDL_INIT_VIDEO) == 1)
		throw string("Could not initialize SDL: ") + SDL_GetError();
}


SDL_Library::~SDL_Library()
{
	SDL_Quit();
}
