#ifdef _CURSES
#include "graphic/curses_lib.h"

#include <vector>
using namespace std;

#include <ncurses.h>

#include "terminal/framebuffer.h"

Curses::Curses(int const w, int const h)
	: GraphicLibrary(w, h)
{
	initscr();
	raw();
	keypad(stdscr, TRUE);
}


Curses::~Curses()
{
	endwin();
}


void
Curses::Update()
{
	for(vector<int>::const_iterator it = fb->dirty.begin(); 
			it < fb->dirty.end(); it++)
	{
		int x = (*it) % fb->W();
		int y = (*it) / fb->W();
		mvaddch(y, x, fb->Ch(x, y).Ch);
	}
	refresh();
}

#endif
