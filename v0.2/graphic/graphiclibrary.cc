#include "graphic/graphiclibrary.h"

#include <cstdlib>
#include <iostream>
using namespace std;

#include "graphic/sdl_library.h"
#include "graphic/curses_lib.h"

GraphicLibrary* 
GraphicLibrary::Initialize(GraphicLibraries const lib, int const w, int const h)
{
	switch(lib)
	{
#ifdef _SDL
	case SDL:
		return new SDL_Library(w, h);
#endif
#ifdef _CURSES
	case CURSES:
		return new Curses(w, h);
#endif
	default:
		cerr << "No graphic libraries were defined." << endl;
		exit(1);
	}
}

void 
GraphicLibrary::Initialize(Options const& options, 
		Framebuffer const& framebuffer)
{
	this->options = &options;
	this->fb = &framebuffer;
}
