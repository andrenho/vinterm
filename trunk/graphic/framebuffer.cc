#include "graphic/framebuffer.h"	

Framebuffer::Framebuffer(Options const& options, CharMatrix const& cm)
	: options(options), cm(cm)
{
}


Framebuffer::~Framebuffer()
{
}
