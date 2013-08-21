#ifndef SIMPLE_H
#define SIMPLE_H

#include "render/renderer.h"

class Simple : public Renderer
{
public:
	void Render(SDL_Renderer*& ren, Framebuffer const& fb) const;
	void Resize(int scr_w, int scr_h, int& ts_w, int& ts_h) const;
};

#endif

