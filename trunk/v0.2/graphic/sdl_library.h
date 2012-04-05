#ifndef SDL_LIBRARY_H
#define SDL_LIBRARY_H

#ifdef _SDL

#include "graphic/graphiclibrary.h"

class SDL_Library : public GraphicLibrary 
{
public:
	SDL_Library(int const w, int const h);
	~SDL_Library();
	void Update() { }
	int Input() const { return 0; }
};

#endif
#endif
