#include "terminal/vinterm.h"

#include <cctype>
#include <sstream>
#include <iostream>

#include "options.h"
#include "terminal/pty.h"
#include "terminal/charmatrix.h"
#include "terminal/keyqueue.h"
#include "terminal/mouse.h"
#include "graphic/audio.h"
#include "graphic/screen.h"


Vinterm::Vinterm()
	: Terminal(), saved_x(0), saved_y(0)
{
}


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
		cm->MoveCursor(Direction::UP, p[0]);
		break;
	case 'B': // move cursor down
		cm->MoveCursor(Direction::DOWN, p[0]);
		break;
	case 'C': // move cursor forward
		cm->MoveCursor(Direction::RIGHT, p[0]);
		break;
	case 'D': // move cursor forward
		cm->MoveCursor(Direction::LEFT, p[0]);
		break;
	case 'H': // move cursor to home or specified position
		cm->SetCursorPosition((p[1] == 0 ? 1 : p[1]), 
				     (p[0] == 0 ? 1 : p[0]));
		break;
	case 'c': // move cursor to absolute column
		cm->SetCursorPosition(p[0]+1, cm->CursorY()+1);
		break;
	/* case 'l': // move cursor to lower left of the screen
		cm->SetCursorPosition(cm->W(), cm->H());
		break; */
	case 's': // save cursor position
		saved_x = cm->CursorX();
		saved_y = cm->CursorY();
		break;
	case 'R': // restore cursor position
		cm->SetCursorPosition(saved_x+1, saved_y+1);
		break;
	case 'U': // scroll reverse one line
		cm->RecedeCursorY();
		break;
	case 'S': // scroll up terminal
		for(int i=0; i<p[0]; i++)
			cm->ScrollUp();
		break;
	case 'r': // set scrolling region
		cm->SetScrollingRegion(p[0], p[1]);
		break;
	case 'L': // add line below cursor
		cm->AddLinesBelowCursor(p[0] == 0 ? 1 : p[0]);
		break;
	case 'K': // erase line
		cm->ClearRow(p[0]);
		break;
	case 'J': // erase everything from cursor
		for(int y = cm->CursorY(); y < cm->H(); y++)
			cm->ClearRow(false, y);
		break;
	case 'M': // delete lines
		cm->DeleteLines(p[0] == 0 ? 1 : p[0]);
		break;
	case 'P': // delete chars
		cm->DeleteChars(p[0] == 0 ? 1 : p[0]);
		break;
	case 'X': // erase chars
		cm->EraseChars(p[0]);
		break;
	case 'I': // insert chars
		cm->InsertChars(p[0] == 0 ? 1 : p[0]);
		break;
	case 'w': // get into insert mode
		cm->InsertMode = p[0];
		break;
	case 'y': // save/restore screen
		if(p[0] == 1)
			cm->SaveScreen();
		else if(p[0] == 0)
			cm->RestoreScreen();
		else
			Terminal::ExecuteEscapeSequence(sequence);
		break;
	case 'm': // enable character attributes
		for(vector<int>::iterator it = p.begin(); it < p.end(); ++it)
		{
			if(*it == 0 && it == p.begin())
				cm->SetAttr(NONE, true);
			else if(*it == 1)
				cm->SetAttr(HIGHLIGHT, true);
			else if(*it == 2)
				cm->SetAttr(DIM, true);
			else if(*it == 4)
				cm->SetAttr(UNDERLINE, true);
			else if(*it == 5)
				cm->SetAttr(BLINK, true);
			else if(*it == 7)
				cm->SetAttr(REVERSE, true);
			else if(*it == 8)
				cm->SetAttr(INVISIBLE, true);
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
				cm->SetAttr(HIGHLIGHT, false);
			else if(*it == 2)
				cm->SetAttr(DIM, false);
			else if(*it == 4)
				cm->SetAttr(UNDERLINE, false);
			else if(*it == 5)
				cm->SetAttr(BLINK, false);
			else if(*it == 7)
				cm->SetAttr(REVERSE, false);
			else if(*it == 8)
				cm->SetAttr(INVISIBLE, false);
			else if(*it != 0)
			{
				Terminal::ExecuteEscapeSequence(sequence);
				break;
			}
		}
		break;
	case 'u': // cursor visibility
		switch(p[0])
		{
			case 0: cm->CursorVisibility = NOT_VISIBLE; break;
			case 1: cm->CursorVisibility = VISIBLE; break;
			case 2: cm->CursorVisibility = VERY_VISIBLE; break;
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
	case 'F': // flash (visible bell)
		cm->Flash();
		break;
	case 'h': // mouse mode
		mouse->SetMode(p[0]);
		break;
	case 'l': // reset mode
		mouse->ResetMode(p[0]);
		break;
	case 'E': // start reading window name
		readingStatusLine = true;
		cm->setTerminalTitle("");
		break;
	case 'G': // stop reading window name
		readingStatusLine = false;
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
		cm->Put((char)254, false); break;
	case 'a': // checker board
		cm->Put((char)177, false); break;
	case 'f': // degree symbol
		cm->Put((char)248, false); break;
	case 'g': // plus/minus
		cm->Put((char)241, false); break;
	case 'i': // lantern
		cm->Put((char)173, false); break;
	case 'j': // lower right corner
		cm->Put((char)217, false); break;
	case 'k': // upper right corner
		cm->Put((char)191, false); break;
	case 'l': // upper left corner
		cm->Put((char)218, false); break;
	case 'm': // lower left corner
		cm->Put((char)192, false); break;
	case 'n': // plus
		cm->Put('+', false); break;
	case 'o': // scan line 1
		cm->Put((char)238, false); break;
	case 'p': // scan line 3
		cm->Put((char)238, false); break;
	case 'q': // horizontal line
		cm->Put((char)196, false); break;
	case 'r': // scan line 6
		cm->Put((char)196, false); break;
	case 's': // scan line 9
		cm->Put((char)196, false); break;
	case 't': // left tee
		cm->Put((char)195, false); break;
	case 'u': // right tee
		cm->Put((char)180, false); break;
	case 'v': // bottom tee
		cm->Put((char)193, false); break;
	case 'w': // top tee (T)
		cm->Put((char)194, false); break;
	case 'x': // vertical line
		cm->Put((char)179, false); break;
	case ',': // left arrow
		cm->Put((char)174, false); break;
	case '+': // right arrow
		cm->Put((char)175, false); break;
	case 'y': // less or eq
		cm->Put((char)254, false); break;
	case 'z': // more or eq
		cm->Put((char)254, false); break;
	case '{': // pi
		cm->Put((char)244, false); break;
	case '|': // not equal
		cm->Put((char)254, false); break;
	case '}': // uk pound
		cm->Put((char)156, false); break;
	case '~': // bullet
		cm->Put((char)250, false); break;
	case '0': // solid square
		cm->Put((char)219, false); break;
	default:
		cm->Put(c, false);
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
			pty->Send(s.str());
		}
		break;
	case K_UP:      pty->Send("\033[A"); break;
	case K_DOWN:    pty->Send("\033[B"); break;
	case K_RIGHT:   pty->Send("\033[C"); break;
	case K_LEFT:    pty->Send("\033[D"); break;
	case SH_UP:     pty->Send("\033[a"); break;
	case SH_DOWN:   pty->Send("\033[b"); break;
	case SH_RIGHT:  pty->Send("\033[c"); break;
	case SH_LEFT:   pty->Send("\033[d"); break;
	case HOME:      pty->Send("\033[1~"); break;
	case DELETE:    pty->Send("\033[3~"); break;
	case PAGE_UP:   pty->Send("\033[5~"); break;
	case PAGE_DOWN: pty->Send("\033[6~"); break;
	case INSERT:    pty->Send("\033[2~"); break;
	case END:       pty->Send("\033[4~"); break;

	case CT_PAGE_UP:   pty->Send("\033[5;5~"); break;
	case CT_PAGE_DOWN: pty->Send("\033[6;5~"); break;

	default:
		Terminal::KeyPressed(key);
	}
}
