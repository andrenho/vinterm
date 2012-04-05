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


//void
//Framebuffer::operator<<(const char c)
istream& 
operator>>(istream& in, Framebuffer& fb)
{
	char c;
	in >> c;

	// put char on the grid
	int pos = fb.cursor_x + (fb.cursor_y * fb.W());
	fb.chars[pos].Ch = c;
	fb.chars[pos].Attr = fb.current_attr;

	// add to list of positions to be updated on the screen
	fb.dirty->push_back(pos);

	// advance cursor
	fb.AdvanceCursorX();

	return in;
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
