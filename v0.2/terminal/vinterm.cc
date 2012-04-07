#include "terminal/vinterm.h"

#include <cctype>

#include "terminal/framebuffer.h"

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
	default:
		Terminal::ExecuteEscapeSequence(sequence);
	}
}


char 
Vinterm::ParseEscapeSequence(string const& sequence, vector<int>& params)
{
	char cmd = sequence[sequence.length()-1];

	for(string::const_iterator c = sequence.begin();
			c < sequence.end(); c++)
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
