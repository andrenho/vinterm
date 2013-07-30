#include "terminal/vinterm.h"

#include <cctype>
#include <sstream>
#include <iostream>

#include "graphic/screen.h"
#include "terminal/framebuffer.h"
#include "terminal/pty.h"

void 
Vinterm::ExecuteEscapeSequence(string const& sequence)
{
	vector<int> p;
	char command = ParseEscapeSequence(sequence, p);

	if(p.size() <= 2)
	{
		p.push_back(0);
		p.push_back(0);
	}

	switch(command)
	{
	case 'A': // move cursor up
		fb.MoveCursor(Direction::UP, p[0]);
		break;
	case 'B': // move cursor down
		fb.MoveCursor(Direction::DOWN, p[0]);
		break;
	case 'C': // move cursor forward
		fb.MoveCursor(Direction::RIGHT, p[0]);
		break;
	case 'D': // move cursor forward
		fb.MoveCursor(Direction::LEFT, p[0]);
		break;
	case 'H': // move cursor to home or specified position
		fb.SetCursorPosition((p[1] == 0 ? 1 : p[1]), 
				     (p[0] == 0 ? 1 : p[0]));
		break;
	case 'c': // move cursor to absolute column
		fb.SetCursorPosition(p[0]+1, fb.CursorY()+1);
		break;
	case 'l': // move cursor to lower left of the screen
		fb.SetCursorPosition(fb.W(), fb.H());
		break;
	case 's': // save cursor position
		saved_x = fb.CursorX();
		saved_y = fb.CursorY();
		break;
	case 'R': // restore cursor position
		fb.SetCursorPosition(saved_x+1, saved_y+1);
		break;
	case 'U': // scroll reverse one line
		fb.RecedeCursorY();
		break;
	case 'S': // scroll up terminal
		for(int i=0; i<p[0]; i++)
			fb.ScrollUp();
		break;
	case 'r': // set scrolling region
		fb.SetScrollingRegion(p[0], p[1]);
		break;
	case 'L': // add line below cursor
		fb.AddLinesBelowCursor(p[0] == 0 ? 1 : p[0]);
		break;
	case 'K': // erase line
		fb.ClearRow(p[0]);
		break;
	case 'J': // erase everything from cursor
		for(int y = fb.CursorY(); y < fb.H(); y++)
			fb.ClearRow(false, y);
		break;
	case 'M': // delete lines
		fb.DeleteLines(p[0] == 0 ? 1 : p[0]);
		break;
	case 'P': // delete chars
		fb.DeleteChars(p[0] == 0 ? 1 : p[0]);
		break;
	case 'X': // erase chars
		fb.EraseChars(p[0]);
		break;
	case 'I': // insert chars
		fb.InsertChars(p[0] == 0 ? 1 : p[0]);
		break;
	case 'w': // get into insert mode
		fb.InsertMode = p[0];
		break;
	case 'y': // save/restore screen
		if(p[0] == 1)
			fb.SaveScreen();
		else if(p[0] == 0)
			fb.RestoreScreen();
		else
			Terminal::ExecuteEscapeSequence(sequence);
		break;
	case 'm': // enable character attributes
		for(vector<int>::iterator it = p.begin(); it < p.end(); ++it)
		{
			if(*it == 0 && it == p.begin())
				fb.SetAttr(NONE, true);
			else if(*it == 1)
				fb.SetAttr(HIGHLIGHT, true);
			else if(*it == 2)
				fb.SetAttr(DIM, true);
			else if(*it == 4)
				fb.SetAttr(UNDERLINE, true);
			else if(*it == 5)
				fb.SetAttr(BLINK, true);
			else if(*it == 7)
				fb.SetAttr(REVERSE, true);
			else if(*it == 8)
				fb.SetAttr(INVISIBLE, true);
			else if(*it != 0)
			{
				Terminal::ExecuteEscapeSequence(sequence);
				break;
			}
		}
		break;
	case 'n': // disable character attributes
		for(vector<int>::iterator it = p.begin(); it < p.end(); ++it)
		{
			if(*it == 1)
				fb.SetAttr(HIGHLIGHT, false);
			else if(*it == 2)
				fb.SetAttr(DIM, false);
			else if(*it == 4)
				fb.SetAttr(UNDERLINE, false);
			else if(*it == 5)
				fb.SetAttr(BLINK, false);
			else if(*it == 7)
				fb.SetAttr(REVERSE, false);
			else if(*it == 8)
				fb.SetAttr(INVISIBLE, false);
			else if(*it != 0)
			{
				Terminal::ExecuteEscapeSequence(sequence);
				break;
			}
		}
		break;
	case 'F': // flash (visible bell)
		fb.Flash();
		break;
	case 'u': // cursor visibility
		switch(p[0])
		{
			case 0: fb.CursorVisibility = NOT_VISIBLE; break;
			case 1: fb.CursorVisibility = VISIBLE; break;
			case 2: fb.CursorVisibility = VERY_VISIBLE; break;
			default: Terminal::ExecuteEscapeSequence(sequence);
		}
		break;
	case 't': // alternate charset
		if(p[0] == 1)
			alternateCharset = true;
		else if(p[0] == 0)
			alternateCharset = false;
		else
			Terminal::ExecuteEscapeSequence(sequence);
		break;
	default:
		Terminal::ExecuteEscapeSequence(sequence);
	}
}


void
Vinterm::InputAlternateChar(const char c)
{
	switch(c)
	{
	case '`': // diamond
		fb.Put((char)254, false); break;
	case 'a': // checker board
		fb.Put((char)177, false); break;
	case 'f': // degree symbol
		fb.Put((char)248, false); break;
	case 'g': // plus/minus
		fb.Put((char)241, false); break;
	case 'i': // lantern
		fb.Put((char)173, false); break;
	case 'j': // lower right corner
		fb.Put((char)217, false); break;
	case 'k': // upper right corner
		fb.Put((char)191, false); break;
	case 'l': // upper left corner
		fb.Put((char)218, false); break;
	case 'm': // lower left corner
		fb.Put((char)192, false); break;
	case 'n': // plus
		fb.Put('+', false); break;
	case 'o': // scan line 1
		fb.Put((char)238, false); break;
	case 'p': // scan line 3
		fb.Put((char)238, false); break;
	case 'q': // horizontal line
		fb.Put((char)196, false); break;
	case 'r': // scan line 6
		fb.Put((char)196, false); break;
	case 's': // scan line 9
		fb.Put((char)196, false); break;
	case 't': // left tee
		fb.Put((char)195, false); break;
	case 'u': // right tee
		fb.Put((char)180, false); break;
	case 'v': // bottom tee
		fb.Put((char)193, false); break;
	case 'w': // top tee (T)
		fb.Put((char)194, false); break;
	case 'x': // vertical line
		fb.Put((char)179, false); break;
	case ',': // left arrow
		fb.Put((char)174, false); break;
	case '+': // right arrow
		fb.Put((char)175, false); break;
	case 'y': // less or eq
		fb.Put((char)254, false); break;
	case 'z': // more or eq
		fb.Put((char)254, false); break;
	case '{': // pi
		fb.Put((char)244, false); break;
	case '|': // not equal
		fb.Put((char)254, false); break;
	case '}': // uk pound
		fb.Put((char)156, false); break;
	case '~': // bullet
		fb.Put((char)250, false); break;
	case '0': // solid square
		fb.Put((char)219, false); break;
	default:
		fb.Put(c, false);
	}
}


char 
Vinterm::ParseEscapeSequence(string const& sequence, vector<int>& params)
{
	char cmd = sequence[sequence.length()-1];

	for(string::const_iterator c = sequence.begin(); c < sequence.end(); c++)
	{
		string s;
		while(isdigit(*c))
		{
			s += *c;
			c++;
		}
		if(!s.empty())
			params.push_back(strtol(s.c_str(), NULL, 10));
	}

	return cmd;
}


void
Vinterm::KeyPressed(uint32_t key)
{
	switch(key)
	{
	case F1: case F2: case F3: case F4: case F5: case F6:
	case F7: case F8: case F9: case F10: case F11: case F12:
		{
			stringstream s;
			s << "\033[" << (key-F1+1) << "k";
			pty.Send(s.str());
		}
		break;
	case K_UP:      pty.Send("\033[A"); break;
	case K_DOWN:    pty.Send("\033[B"); break;
	case K_RIGHT:   pty.Send("\033[C"); break;
	case K_LEFT:    pty.Send("\033[D"); break;
	case SH_UP:     pty.Send("\033[a"); break;
	case SH_DOWN:   pty.Send("\033[b"); break;
	case SH_RIGHT:  pty.Send("\033[c"); break;
	case SH_LEFT:   pty.Send("\033[d"); break;
	case HOME:      pty.Send("\033[1~"); break;
	case DELETE:    pty.Send("\033[3~"); break;
	case PAGE_UP:   pty.Send("\033[5~"); break;
	case PAGE_DOWN: pty.Send("\033[6~"); break;
	case INSERT:    pty.Send("\033[2~"); break;
	case END:       pty.Send("\033[4~"); break;

	case CT_PAGE_UP:   pty.Send("\033[5;5~"); break;
	case CT_PAGE_DOWN: pty.Send("\033[6;5~"); break;

	default:
		Terminal::KeyPressed(key);
	}
}
