#include "render/simple.h"

#include "graphic/framebuffer.h"
#include "graphic/font.h"

Simple::Simple(Font const& font, Framebuffer& fb)
	: Renderer(font, fb), texture(nullptr)
{
}


Simple::~Simple()
{
	if(texture)
		SDL_DestroyTexture(texture);
}


void 
Simple::Resize(int scr_w, int scr_h, int& ts_w, int& ts_h) const
{
	ts_w = scr_w / font.CharWidth();
	ts_h = scr_h / font.CharHeight();
	fb.Resize(scr_w, scr_h, ts_w, ts_h);
	this->w = scr_w;
	this->h = scr_h;

	// create texture
	if(texture)
		SDL_DestroyTexture(texture);
	texture = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING, w, h);
}


void
Simple::Render() const
{
	if(!texture)
		return;

	uint32_t* pixels;
	int pitch;
	SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);

	for(int i=0; i<w*h; i++)
	{
		Color p = fb.palette[fb.Pixels()[i]];
		pixels[i] = (p.r << 16) + (p.g << 8) + p.b;
	}
	SDL_UnlockTexture(texture);
	SDL_RenderClear(ren);
	SDL_RenderCopy(ren, texture, NULL, NULL);
	SDL_RenderPresent(ren);
}
