#include "vt100.h"

#include <cstdlib>
#include <iostream>
using namespace std;

void 
VT100::ExecuteEscapeSequence(string const& seq)
{
	if(seq[1] == '[')
	{
		char command;
		vector<int> parameters;
		if(!ParseCommand(seq, command, parameters))
			InvalidEscapeSequence(seq);
		switch(command)
		{
		case 'A':
			if(parameters.size() == 0)
				MoveCursorUp(1);
			else
				MoveCursorUp(parameters[0]);
			break;
		case 'B':
			if(parameters.size() == 0)
				MoveCursorDown(1);
			else
				MoveCursorDown(parameters[0]);
			break;
		case 'C':
			if(parameters.size() == 0)
				MoveCursorRight(1);
			else
				MoveCursorRight(parameters[0]);
			break;
		case 'D':
			if(parameters.size() == 0)
				MoveCursorLeft(1);
			else
				MoveCursorLeft(parameters[0]);
			break;
		case 'H':
			if(parameters.empty())
				ChangeCursorPosition(0, 0);
			else if(parameters.size() == 2)
				ChangeCursorPosition(parameters[1]-1, parameters[0]-1);
			else
				InvalidEscapeSequence(seq);
			break;
		case 'J':
			if(parameters.empty() || parameters[0] == 0)
				EraseFromCursorToEndOfScreen();
			else
				InvalidEscapeSequence(seq);
			break;
		case 'K':
			EraseToEOL();
			break;
		case 'h':
			if(parameters.size() != 1)
				InvalidEscapeSequence(seq);
			else if(parameters[0] == 1)
				cursor_keys = APPLICATION;
			else
				InvalidEscapeSequence(seq);
			break;
		case 'l':
			if(parameters.size() != 1)
				InvalidEscapeSequence(seq);
			else if(parameters[0] == 1)
				cursor_keys = CURSOR;
			else
				InvalidEscapeSequence(seq);
			break;
		case 'm':
			if(parameters.empty() || parameters[0] == 0)
				current_attr = NORMAL;
			else if(parameters[0] == 4)
				current_attr.Underline = true;
			else if(parameters[0] == 7)
				current_attr.Reverse = true;
			else if(parameters[0] == 1)
				current_attr.Highlight = true;
			else if(parameters[0] == 5)
				current_attr.Blink = true;
			else
				InvalidEscapeSequence(seq);
			break;
		case 'r':
			if(parameters.size() != 2)
				InvalidEscapeSequence(seq);
			else
				SetScrollRegion(parameters[0], parameters[1]);
			break;
		default:
			Terminal::ExecuteEscapeSequence(seq);
		}
	}
	else 
		switch(seq[1])
		{
		case '=':
			keypad_keys = APPLICATION;
			break;
		case '>':
			keypad_keys = CURSOR;
			break;
		case 'M':
			if(cursor_y > 0)
				MoveCursorUp(1);
			else
				ScrollDown();
			break;
		default:
			InvalidEscapeSequence(seq);
		}
}


void 
VT100::SpecialKeyPress(SpecialKey key)
{
	if(key == UP || key == DOWN || key == LEFT || key == RIGHT)
	{
		if(cursor_keys == CURSOR)
			Terminal::SpecialKeyPress(key);
		else
			switch(key)
			{	
			case UP:
				KeyPress(27); KeyPress('O'); KeyPress('A'); break;
			case DOWN:
				KeyPress(27); KeyPress('O'); KeyPress('B'); break;
			case RIGHT:
				KeyPress(27); KeyPress('O'); KeyPress('C'); break;
			case LEFT:
				KeyPress(27); KeyPress('O'); KeyPress('D'); break;
			default:
				abort();
			}
	}
	else
	{
		if(keypad_keys == CURSOR)
			Terminal::SpecialKeyPress(key);
		else
			switch(key)
			{
			case KP0:
				KeyPress(27); KeyPress('O'); KeyPress('p'); break;
			case KP1:
				KeyPress(27); KeyPress('O'); KeyPress('q'); break;
			case KP2:
				KeyPress(27); KeyPress('O'); KeyPress('r'); break;
			case KP3:
				KeyPress(27); KeyPress('O'); KeyPress('s'); break;
			case KP4:
				KeyPress(27); KeyPress('O'); KeyPress('t'); break;
			case KP5:
				KeyPress(27); KeyPress('O'); KeyPress('u'); break;
			case KP6:
				KeyPress(27); KeyPress('O'); KeyPress('v'); break;
			case KP7:
				KeyPress(27); KeyPress('O'); KeyPress('w'); break;
			case KP8:
				KeyPress(27); KeyPress('O'); KeyPress('x'); break;
			case KP9:
				KeyPress(27); KeyPress('O'); KeyPress('y'); break;
			case KP_MINUS:
				KeyPress(27); KeyPress('O'); KeyPress('m'); break;
			case KP_STAR:
				KeyPress(27); KeyPress('O'); KeyPress('l'); break;
			case KP_DOT:
				KeyPress(27); KeyPress('O'); KeyPress('n'); break;
			case KP_PLUS:
				KeyPress(27); KeyPress('O'); KeyPress('M'); break;
			default:
				abort();
			}
	}
}


bool
VT100::ParseCommand(string const& seq, char& command, vector<int>& parameters)
{
	if(seq[seq.length()-1] == 'H')
	{
		// hook
	}

	string::const_iterator it;
	unsigned int i;
	for(i = 0, it = seq.begin(); it < seq.end(); i++, it++)
	{
		char c(*it);
		if(c == 27 || c == '[' || c == '?' || c == ';') // escape
			continue;
		else if(c == '(' || c == ')')
		{
			command = c;
		}
		if((c >= 64 && c <= 126) || i == seq.size()-1) // Command
		{
			command = c;
			return true;
		}
		else
		{
			string par;
			while(((*it) < 64 || (*it) > 126) && (*it) != ';')
			{
				par += (*it);
				it++;
			}

			// TODO - replace for a C++ conversion function
			int n = strtol(par.c_str(), NULL, 10);
			parameters.push_back(n);
			it--;
		}
	}

	return false;
}


void
VT100::ChangeCursorPosition(const int x, const int y)
{
	cursor_x = x;
	cursor_y = y;
	/*
	while(cursor_y >= scroll_bottom)
	{
		ScrollUp();
		--cursor_y;
	}*/
	UpdateCursorPosition();
}


void
VT100::EraseFromCursorToEndOfScreen()
{
	for(int x(0); x<w; x++)
		for(int y(0); y<h; y++)
			SetChar(x, y, ' ', NORMAL);
}


void
VT100::EraseToEOL()
{
	for(int x(cursor_x); x<w; x++)
		SetChar(x, cursor_y, ' ', NORMAL);
}


void 
VT100::MoveCursorUp(const int n)
{
	ChangeCursorPosition(cursor_x, max(0, cursor_y - n));
}


void 
VT100::MoveCursorDown(const int n)
{
	ChangeCursorPosition(cursor_x, min(w-1, cursor_y + n));
}


void 
VT100::MoveCursorLeft(const int n)
{
	ChangeCursorPosition(max(0, cursor_x - n), cursor_y);
}


void 
VT100::MoveCursorRight(const int n)
{
	ChangeCursorPosition(min(w-1, cursor_x + n), cursor_y);
}
