#include "render/simple.h"

#include <iostream>
using namespace std;

#include "graphic/font.h"
#include "graphic/framebuffer.h"
#include "util/chronometer.h"

Simple::Simple()
	: Renderer(), texture(nullptr)
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
	ts_w = scr_w / font->CharWidth();
	ts_h = scr_h / font->CharHeight();
	framebuffer->Resize(scr_w, scr_h, ts_w, ts_h);
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

	ch->Next("render lock texture");
	SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);
	
	ch->Next("render update texture");
	for(Rectangle r : framebuffer->rects)
	{
		for(int x=r.x; x<(r.x+r.w); x++)
			for(int y=r.y; y<(r.y+r.h); y++)
			{
				if(x > framebuffer->W() || y > framebuffer->H())
					break;
				int i = x + (y * w);
				Color p = framebuffer->palette[framebuffer->Pixels()[i]];
				pixels[i] = (p.r << 16) + (p.g << 8) + p.b;
			}
	}
	framebuffer->rects.clear();

	ch->Next("render unlock texture");
	SDL_UnlockTexture(texture);

	ch->Next("render clear");
	SDL_RenderClear(ren);
	ch->Next("render copy texture");
	SDL_RenderCopy(ren, texture, NULL, NULL);
	ch->Next("render present texture");
	SDL_RenderPresent(ren);
}


void
Simple::CharPosition(int mx, int my, int& x, int& y) const
{
	x = mx / font->CharWidth();
	y = my / font->CharHeight();
}
