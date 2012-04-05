#include "terminal/framebuffer.h"

Framebuffer::Framebuffer(Options const& options)
	: options(options), w(80), h(24), cursor_x(0), cursor_y(0)
{
	chars.insert(chars.begin(), w*h, Char());
}


Framebuffer::~Framebuffer()
{
	chars.clear();
}


void
Framebuffer::operator<<(char c)
{
	// put char on the grid
	int pos = cursor_x + (cursor_y * W());
	chars[pos].Ch = c;
	chars[pos].Attr = current_attr;

	// add to list of positions to be updated on the screen
	dirty.push_back(pos);

	// advance cursor
	AdvanceCursorX();
}


void 
Framebuffer::AdvanceCursorX()
{
	cursor_x++; // TODO
}
