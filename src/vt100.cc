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
			else if(parameters[0] == 2)
				EraseScreen();
			else
				InvalidEscapeSequence(seq);
			break;
		case 'K':
			EraseToEOL();
			break;
		case 'c':
			if(parameters.empty() || parameters[0] == 0)
				console->SendString("\033[?1;0c");
			else
				InvalidEscapeSequence(seq);
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
				SetScrollRegion(1, 25);
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
			if(cursor_y > scroll_top)
				MoveCursorUp(1);
			else
				ScrollDown();
			break;
		default:
			InvalidEscapeSequence(seq);
		}
}


bool
VT100::SpecialKeyPress(SDL_keysym key)
{
	if(key.sym == SDLK_UP || key.sym == SDLK_DOWN 
	|| key.sym == SDLK_LEFT || key.sym == SDLK_RIGHT)
	{
		if(cursor_keys == CURSOR)
			return Terminal::SpecialKeyPress(key);
		else
			switch(key.sym)
			{	
			case SDLK_UP:
				KeyPress(27); KeyPress('O'); KeyPress('A'); break;
			case SDLK_DOWN:
				KeyPress(27); KeyPress('O'); KeyPress('B'); break;
			case SDLK_RIGHT:
				KeyPress(27); KeyPress('O'); KeyPress('C'); break;
			case SDLK_LEFT:
				KeyPress(27); KeyPress('O'); KeyPress('D'); break;
			default:
				abort();
			}
	}
	else
	{
		if(keypad_keys == CURSOR)
			return Terminal::SpecialKeyPress(key);
		else
			switch(key.sym)
			{
			case SDLK_KP0:
				KeyPress(27); KeyPress('O'); KeyPress('p'); 
				break;
			case SDLK_KP1:
				KeyPress(27); KeyPress('O'); KeyPress('q'); 
				break;
			case SDLK_KP2:
				KeyPress(27); KeyPress('O'); KeyPress('r'); 
				break;
			case SDLK_KP3:
				KeyPress(27); KeyPress('O'); KeyPress('s'); 
				break;
			case SDLK_KP4:
				KeyPress(27); KeyPress('O'); KeyPress('t'); 
				break;
			case SDLK_KP5:
				KeyPress(27); KeyPress('O'); KeyPress('u');
				break;
			case SDLK_KP6:
				KeyPress(27); KeyPress('O'); KeyPress('v'); 
				break;
			case SDLK_KP7:
				KeyPress(27); KeyPress('O'); KeyPress('w'); 
				break;
			case SDLK_KP8:
				KeyPress(27); KeyPress('O'); KeyPress('x'); 
				break;
			case SDLK_KP9:
				KeyPress(27); KeyPress('O'); KeyPress('y'); 
				break;
			case SDLK_KP_MINUS:
				KeyPress(27); KeyPress('O'); KeyPress('m');
				break;
			case SDLK_KP_MULTIPLY:
				KeyPress(27); KeyPress('O'); KeyPress('l'); 
				break;
			case SDLK_KP_PERIOD:
				KeyPress(27); KeyPress('O'); KeyPress('n'); 
				break;
			case SDLK_KP_PLUS:
				KeyPress(27); KeyPress('O'); KeyPress('M'); 
				break;
			default:
				return Terminal::SpecialKeyPress(key);
			}
	}
	return true;
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
VT100::EraseScreen()
{
	for(int x(0); x<w; x++)
		for(int y(0); y<h; y++)
			SetChar(x, y, ' ', NORMAL);
}


void
VT100::EraseFromCursorToEndOfScreen()
{
	for(int x(0); x<w; x++)
		for(int y(cursor_y); y<h; y++)
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
