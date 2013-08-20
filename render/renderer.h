#ifndef RENDERER_H
#define RENDERER_H

#include "SDL2/SDL.h"

class Framebuffer;

class Renderer
{
public:
	virtual void Render(SDL_Renderer*& ren, Framebuffer const& fb) const = 0;
};

#endif
