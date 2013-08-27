#ifndef RENDERER_H
#define RENDERER_H

#include "SDL2/SDL.h"

#include "global.h"

class Renderer
{
public:
	virtual ~Renderer() { }

	virtual void Render() const = 0;
	virtual void Resize(int scr_w, int scr_h, int& ts_w, int& ts_h) const = 0;
	virtual void CharPosition(int mx, int my, int& x, int& y) const = 0;

protected:
	Renderer();

	SDL_Renderer* ren;
};

#endif
