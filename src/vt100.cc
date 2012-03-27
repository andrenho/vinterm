#include "vt100.h"

#include <cstdlib>
#include <iostream>
using namespace std;

void 
VT100::ExecuteEscapeSequence(string const& seq)
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
	case 'M':
		if(!parameters.empty())
			InvalidEscapeSequence(seq);
		if(cursor_y > 0)
			MoveCursorUp(1);
		else
			ScrollDown();
		break;
	case 'm':
		if(parameters.empty() || parameters[0] == 0)
			SetAttribute(NORMAL);
		/*
		else if(parameters[0] == "4")
			SetAttribute(UNDERLINE);
		else if(parameters[0] == "7")
			SetAttribute(REVERSE);
		else if(parameters[0] == "1")
			SetAttribute(HIGHLIGHT);
		else if(parameters[0] == "5")
			SetAttribute(BLINK); */
		else
			InvalidEscapeSequence(seq);
		break;
	default:
		Terminal::ExecuteEscapeSequence(seq);
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
	for(it = seq.begin(); it < seq.end(); it++)
	{
		char c(*it);
		if(c == 27 || c == '[' || c == '?' || c == ';') // escape
			continue;
		if(c >= 64 && c <= 126) // Command
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
VT100::SetAttribute(CharAttr attr)
{
	// TODO
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
