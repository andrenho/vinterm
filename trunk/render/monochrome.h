#ifndef MONOCHROME_H
#define MONOCHROME_H

#include "render/renderer.h"

class Monochrome : public Renderer
{
public:
	virtual void Render(SDL_Renderer*& ren, Framebuffer const& fb) const;
};

#endif
