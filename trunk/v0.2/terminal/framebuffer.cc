#include "terminal/framebuffer.h"

#include <cstdlib>

Framebuffer::Framebuffer()
	: dirty(new set<int>), w(80), h(24), 
	  cursor_x(0), cursor_y(0), scroll_top(0), scroll_bottom(h-1)
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
	Put(c, cursor_x, cursor_y);
	AdvanceCursorX();
}


void 
Framebuffer::Put(const char c, const int x, const int y)
{
	Put(c, current_attr, x, y);
}


void 
Framebuffer::Put(const char c, Attribute attr, const int x, const int y)
{
	// put char on the grid
	int pos = x + (y * W());
	chars[pos].Ch = c;
	chars[pos].Attr = attr;

	// add to list of positions to be updated on the screen
	dirty->insert(pos);
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
Framebuffer::CarriageReturn()
{
	cursor_x = 0;
}


void
Framebuffer::Tab()
{
	AdvanceCursorX();
	while((cursor_x % 8) != 0)
		AdvanceCursorX();
}


void
Framebuffer::Backspace()
{
	if(cursor_x > 0)
		--cursor_x;
}


void
Framebuffer::ScrollUp()
{
	for(int y(scroll_top+1); y<=scroll_bottom; y++)
		for(int x(0); x<w; x++)
			Put(Ch(x,y).Ch, Ch(x,y).Attr, x, y-1);
	for(int x(0); x<w; x++)
		Put(' ', Attribute(), x, scroll_bottom);
}


void
Framebuffer::ScrollDown()
{
	for(int y(scroll_bottom-1); y>=scroll_top; y--)
		for(int x(0); x<w; x++)
			Put(Ch(x,y).Ch, Ch(x,y).Attr, x, y+1);
	for(int x(0); x<w; x++)
		Put(' ', Attribute(), x, scroll_top);
}


void 
Framebuffer::MoveCursor(Direction dir, int moves)
{
	int x = 0, y = 0;
	switch(dir)
	{
		case UP: y = -1; break;
		case DOWN: y = 1; break;
		case LEFT: x = -1; break;
		case RIGHT: x = 1; break;
		default: abort();
	}

	if(moves == 0)
		moves = 1;

	cursor_x += (x * moves);
	cursor_y += (y * moves);

	if(cursor_x < 0)
	{
		fprintf(stderr, "warning: application tried to move cursor to x < 0.\n");
		cursor_x = 0;
	}
	else if(cursor_x >= w)
	{
		fprintf(stderr, "warning: application tried to move cursor to x >= w.\n");
		cursor_x = w-1;
	}
	if(cursor_y < 0)
	{
		fprintf(stderr, "warning: application tried to move cursor to y < 0.\n");
		cursor_y = 0;
	}
	else if(cursor_y >= h)
	{
		fprintf(stderr, "warning: application tried to move cursor to y >= h.\n");
		cursor_y = h-1;
	}
}


void
Framebuffer::Home()
{
	cursor_x = cursor_y = 0;
}


void
Framebuffer::LowerLeft()
{
	cursor_x = w-1;
	cursor_y = h-1;
}
