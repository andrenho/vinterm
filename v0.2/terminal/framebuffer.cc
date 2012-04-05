#include "terminal/framebuffer.h"

Framebuffer::Framebuffer(Options const& options)
	: dirty(new vector<int>), options(options), w(80), h(24), 
	  cursor_x(0), cursor_y(0), scroll_top(0), scroll_bottom(23)
{
	chars.insert(chars.begin(), w*h, Char());
}


Framebuffer::~Framebuffer()
{
	chars.clear();
	dirty->clear();
	delete dirty;
}


void 
Framebuffer::Put(const char c)
{
	// put char on the grid
	int pos = cursor_x + (cursor_y * W());
	chars[pos].Ch = c;
	chars[pos].Attr = current_attr;

	// add to list of positions to be updated on the screen
	dirty->push_back(pos);

	// advance cursor
	AdvanceCursorX();
}


void 
Framebuffer::AdvanceCursorX()
{
	++cursor_x;

	if(cursor_x >= w)
	{
		AdvanceCursorY();
		cursor_x = 0;
	}
}


void 
Framebuffer::AdvanceCursorY()
{
	++cursor_y;

	// advance page
	if(cursor_y > scroll_bottom)
	{
		ScrollUp();
		--cursor_y;
	}
}


void
Framebuffer::ScrollUp()
{
	// TODO
}
