#include "vt100.h"

#include <iostream>
using namespace std;

void 
VT100::ExecuteEscapeSequence(string const& seq)
{
	char command;
	vector<string> parameters;
	if(!ParseCommand(seq, command, parameters))
		InvalidEscapeSequence(seq);

	switch(command)
	{
	case 'H':
		ChangeCursorPosition(0, 0);
		break;
	case 'J':
		if(parameters.empty() || parameters[0] == "0")
			EraseFromCursorToEndOfScreen();
		else
			InvalidEscapeSequence(seq);
		break;
	case 'm':
		if(parameters.empty() || parameters[0] == "0")
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
VT100::ParseCommand(string const& seq, char& command, vector<string> parameters)
{
	string::const_iterator it;
	for(it = seq.begin(); it < seq.end(); it++)
	{
		char c(*it);
		if(c == 27 || c == '[') // escape
			continue;
		if(c >= 64 || c <= 126) // Command
		{
			command = c;
			return true;
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
