#include "render/simple.h"

#include "graphic/framebuffer.h"
#include "graphic/font.h"

Simple::Simple(Font const& font)
	: Renderer(font)
{
}


void 
Simple::Resize(int scr_w, int scr_h, int& ts_w, int& ts_h,
		Framebuffer& fb) const
{
	ts_w = scr_w / font.CharWidth();
	ts_h = scr_w / font.CharHeight();
	fb.Resize(scr_w, scr_h);
}


void
Simple::Render(SDL_Renderer*& ren, Framebuffer const& fb) const
{
	SDL_Texture* tx = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING, 400, 400);
	uint32_t* pixels = new uint32_t[400 * 400];
	pixels[10+10*400] = 0xffffffff;
	SDL_UpdateTexture(tx, NULL, pixels, 400 * sizeof(uint32_t));
	SDL_RenderClear(ren);
	SDL_RenderCopy(ren, tx, NULL, NULL);
	SDL_RenderPresent(ren);
	delete[] pixels;
}
