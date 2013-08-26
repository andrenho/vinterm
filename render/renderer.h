#ifndef RENDERER_H
#define RENDERER_H

#include "SDL2/SDL.h"

#include "graphic/framebuffer.h"

class Font;

class Renderer
{
public:
	virtual ~Renderer() { }

	virtual void Render() const = 0;
	virtual void Resize(int scr_w, int scr_h, int& ts_w, int& ts_h) const = 0;
	virtual void CharPosition(int mx, int my, int& x, int& y) const = 0;

	void setRenderer(SDL_Renderer* r) { ren = r; }
	::Framebuffer& Framebuffer() const { return fb; }

protected:
	Renderer(Font const& font, ::Framebuffer& fb)
		: font(font), fb(fb) { }

	Font const& font;
	::Framebuffer& fb;
	SDL_Renderer* ren;
};

#endif
