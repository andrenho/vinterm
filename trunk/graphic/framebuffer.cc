#include "graphic/framebuffer.h"

#include <cstdint>
#include <iostream>

#include "terminal/blink.h"
#include "terminal/charmatrix.h"
#include "terminal/charattr.h"
#include "graphic/font.h"

Framebuffer::Framebuffer(Options const& options, CharMatrix& cm, 
		Font const& font)
	: options(options), cm(cm), font(font), w(0), h(0), pixels(nullptr)
{
	for(int i=0; i<256; i++)
		palette[i] = { 0, 0, 0 };
	palette[1] = { 30, 30, 30 };
	palette[2] = { 60, 60, 60 };
	palette[255] = { 255, 255, 255 };
}


Framebuffer::~Framebuffer()
{
	if(pixels)
		delete[] pixels;
}


void 
Framebuffer::Resize(int w, int h, int ts_w, int ts_h)
{
	cm.Resize(ts_w, ts_h);

	if(pixels)
		delete[] pixels;
	this->w = w;
	this->h = h;
	pixels = new uint8_t[w*h];
	for(int i=0; i<(w*h); i++)
		pixels[i] = 255;
}


void
Framebuffer::DrawChars()
{
	set<int>::const_iterator n;
	for(n = cm.dirty->begin(); n != cm.dirty->end(); n++)
	{
		// get framebuffer positions
		int x = (*n) % cm.W();
		int y = (*n) / cm.W();

		// verify if it's the cursor
		bool cursor = (x == cm.CursorX() && y == cm.CursorY());
		if(cm.CursorVisibility == NOT_VISIBLE)
			cursor = false;

		// get character to draw and adjust attribute
		Char ch = cm.Ch(x, y);
		char c = ch.Ch;

		Attribute attr = ch.Attr;
		if(attr.Blink || cursor)
		{
			if(cm.blink->State())
				attr.Reverse = !attr.Reverse;
			else if(!cursor)
				c = ' ';
		}
		else if(attr.Invisible)
			c = ' ';
		if(cursor && cm.CursorVisibility == VERY_VISIBLE)
			attr.Highlight = true;
		if(cm.IsSelected(x, y))
			attr.Reverse = !attr.Reverse;

		// find position on the screen
		int xx = (x * font.CharWidth());
		int yy = (y * font.CharHeight());

		// draw chars
		int fg = 1;
		if(attr.Highlight)
			fg = 0;
		else if(attr.Dim)
			fg = 2;
		int bg = 255;
		if(attr.Reverse)
			swap(fg, bg);
		font.DrawChar(c, pixels, xx, yy, w, bg, fg);
		// TODO - underline

		// register char location for later update
		// TODO - rects.push_back(r);

	}
	cm.dirty->clear();
}
