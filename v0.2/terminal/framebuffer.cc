#include "terminal/framebuffer.h"

#include <cstdlib>

Framebuffer::Framebuffer()
	: dirty(new set<int>), InsertMode(false), w(80), h(24), 
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
Framebuffer::Put(const char c, bool ignore_insert_mode)
{
	if(cursor_x >= w)
	{
		cursor_x = 0;
		AdvanceCursorY();
	}
	ValidateCursorPosition();

	Put(c, cursor_x, cursor_y, ignore_insert_mode);
	
	++cursor_x;
}


void 
Framebuffer::Put(const char c, const int x, const int y, 
		bool ignore_insert_mode)
{
	Put(c, current_attr, x, y, ignore_insert_mode);
}


void 
Framebuffer::Put(const char c, Attribute attr, const int x, const int y,
			bool ignore_insert_mode)
{
	if(InsertMode && !ignore_insert_mode)
		InsertChars(1);

	// put char on the grid
	int pos = x + (y * W());
	chars[pos].Ch = c;
	chars[pos].Attr = attr;

	// add to list of positions to be updated on the screen
	dirty->insert(pos);
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
	ValidateCursorPosition();
}


void Framebuffer::RecedeCursorY()
{
	--cursor_y;
	
	if(cursor_y < scroll_top)
	{
		ScrollDown();
		++cursor_y;
	}
	ValidateCursorPosition();
}


void
Framebuffer::CarriageReturn()
{
	cursor_x = 0;
}


void
Framebuffer::Tab()
{
	++cursor_x;
	while((cursor_x % 8) != 0)
	{
		++cursor_x;
		if(cursor_x >= w)
		{
			cursor_x = 0;
			AdvanceCursorY();
		}
	}
}


void
Framebuffer::Backspace()
{
	if(cursor_x > 0)
		--cursor_x;
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

	ValidateCursorPosition();
}


void 
Framebuffer::SetCursorPosition(int x, int y)
{
	cursor_x = x-1;
	cursor_y = y-1;
	ValidateCursorPosition();
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
Framebuffer::SetScrollingRegion(int top, int bottom)
{
	scroll_top = top-1;
	scroll_bottom = bottom-1;
	cursor_x = cursor_y = 0;
}


void 
Framebuffer::AddLinesBelowCursor(int n)
{
	int save_sc_top = scroll_top;
	int save_sc_bt = scroll_bottom;
	scroll_top = cursor_y;
	scroll_bottom = h-1;
	for(int i=0; i<n; i++)
		ScrollDown();
	scroll_top = save_sc_top;
	scroll_bottom = save_sc_bt;
}


void 
Framebuffer::ClearRow(bool upto_cursor, int y)
{
	if(y == -1) 
		y = cursor_y;
	for(int x = (upto_cursor ? 0 : cursor_x); 
			x <= (upto_cursor ? cursor_x : w-1); x++)
		Put(' ', Attribute(), x, y);
}


void
Framebuffer::DeleteLines(int n)
{
	int save_sc_top = scroll_top;
	int save_sc_bt = scroll_bottom;
	scroll_top = cursor_y;
	scroll_bottom = h-1;
	for(int i=0; i<n; i++)
		ScrollUp();
	scroll_top = save_sc_top;
	scroll_bottom = save_sc_bt;
}


void Framebuffer::DeleteChars(int n)
{
	for(int x=cursor_x; x <= w-n; x++)
	{
		Char ch = Ch(x+n, cursor_y);
		Put(ch.Ch, ch.Attr, x, cursor_y);
	}
	for(int x=w-n; x<w; x++)
		Put(' ', x, cursor_y); // XXX - attribute?
}


void Framebuffer::EraseChars(int n)
{
	for(int x=cursor_x; x<(cursor_x+n); x++)
		Put(' ', x, cursor_y); // XXX - attribute?
}


void 
Framebuffer::InsertChars(int n)
{
	for(int x=w-n-1; x >= cursor_x; x--)
	{
		Char ch = Ch(x, cursor_y);
		Put(ch.Ch, ch.Attr, x+n, cursor_y);
	}
	for(int x=cursor_x; x<(cursor_x+n); x++)
		Put(' ', current_attr, x, cursor_y); // XXX - attribute?
}


void
Framebuffer::ValidateCursorPosition()
{
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
