#include "graphic/framebuffer.h"

#include <cstdint>

Framebuffer::Framebuffer(Options const& options, CharMatrix const& cm)
	: options(options), cm(cm), w(0), h(0), pixels(nullptr)
{
	Resize(400, 400);
}


Framebuffer::~Framebuffer()
{
	if(pixels)
		delete[] pixels;
}


void 
Framebuffer::Resize(int w, int h)
{
	if(pixels)
		delete[] pixels;
	pixels = new uint32_t[w*h];
	pixels[10+10*400] = 0xffffffff;
}
