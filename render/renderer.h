#ifndef RENDERER_H
#define RENDERER_H

#include "SDL2/SDL.h"

class Framebuffer;
class Font;

class Renderer
{
public:
	Renderer(Font const& font)
		: font(font) { }

	virtual void Render(SDL_Renderer*& ren, Framebuffer const& fb) const = 0;
	virtual void Resize(int scr_w, int scr_h, int& ts_w, int& ts_h,
			Framebuffer& fb) const = 0;

protected:
	Font const& font;
};

#endif
