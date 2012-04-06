#include "terminal/terminal.h"

#include <iostream>
using namespace std;

#include "terminal/framebuffer.h"
#include "terminal/pty.h"
#include "graphic/screen.h"

Terminal::Terminal(Framebuffer& fb, PTY& pty)
	: fb(fb), pty(pty), active(true), escape_mode(false)
{
}


Terminal::~Terminal()
{
}


void
Terminal::Input()
{
	// read data from the PTY
	int i;

	while((i = pty.Get()) != PTY::NO_DATA)
	{
		if(i == EOF)
		{
			active = false;
			return;
		}

		const char c = (const char)i;
		if(escape_mode)
		{
			escape_sequence += c;
			if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
			{
				escape_mode = false;
				ExecuteEscapeSequence(escape_sequence);
			}
		}
		else
		{
			switch(c)
			{
			case 27:
				escape_mode = true;
				escape_sequence = "\033";
				break;
			case '\n': // new line
				fb.AdvanceCursorY();
				break;
			case '\r': // carriage return
				fb.CarriageReturn();
				break;
			default:
				fb.Put(c);
			}
		}
	}
}


void 
Terminal::Output(Screen const& screen)
{
	// write data to the PTY
	int ch = screen.Input();

	switch(ch)
	{
	case 0: // discard
		break;
	case Keys::QUIT:
		active = false;
		break;
	default:
		pty.Send((const char)ch);
	}
}


void 
Terminal::ExecuteEscapeSequence(string const& sequence)
{
	cerr << "warning: Unrecognized escape sequence: ";
	for(string::const_iterator c = sequence.begin(); c < sequence.end(); c++)
		if(*c == 27)
			cerr << "ESC";
		else if(*c < 32)
			cerr << "(" << (int)*c << ")";
		else
			cerr << *c;
	cerr << endl;
}
