#include "graphic/framebuffer.h"

#include <cstdint>

Framebuffer::Framebuffer(Options const& options, CharMatrix const& cm)
	: options(options), cm(cm), w(0), h(0), pixels(nullptr)
{
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
	pixels = new uint8_t[w*h];
}
