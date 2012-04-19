#include "terminal/terminal.h"

#include <iostream>
using namespace std;

#include "terminal/framebuffer.h"
#include "terminal/pty.h"
#include "graphic/screen.h"
#include "graphic/audio.h"

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
	fb.Flash(false);

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
			InputEscapeChar(c);
		else
			InputChar(c);
	}
}


void
Terminal::InputChar(const char c)
{
	switch(c)
	{
	case 27: // ESC
		escape_mode = true;
		escape_sequence = "\033";
		break;
	case '\n': // new line
		fb.AdvanceCursorY();
		break;
	case '\r': // carriage return
		fb.CarriageReturn();
		break;
	case '\t': // tab
		fb.Tab();
		break;
	case '\a': // beep
		cout << "Beep!" << endl; // TODO
		{
			Audio a;
			a.Beep();
		}
		break;
	case '\b': // backspace
		fb.Backspace();
		break;
	default:
		fb.Put(c, false);
	}
}

void 
Terminal::InputEscapeChar(const char c)
{
	escape_sequence += c;
	if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
	{
		escape_mode = false;
		ExecuteEscapeSequence(escape_sequence);
	}
}


void 
Terminal::Resize(int new_w, int new_h)
{
	fb.Resize(new_w, new_h);
	pty.Resize(new_w, new_h);
}


void 
Terminal::Output(Screen& screen)
{
	if(screen.keyQueue.empty())
		return;

	// write data to the PTY
	int w, h, fs, ts_w, ts_h;
	int ch = screen.keyQueue[0];
	screen.keyQueue.pop_front();

	switch(ch)
	{
	case 0: // discard
		break;
	case RESIZE:
		w = screen.keyQueue[0];
		screen.keyQueue.pop_front();
		h = screen.keyQueue[0];
		screen.keyQueue.pop_front();
		fs = screen.keyQueue[0];
		screen.keyQueue.pop_front();
		screen.Resize(w, h, fs, ts_w, ts_h);
		Resize(ts_w, ts_h);
		break;
	case QUIT:
		active = false;
		break;
	default:
		KeyPressed(ch);
	}
}


void
Terminal::KeyPressed(int ch)
{
	pty.Send((const char)ch);
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


void 
Terminal::SetEncoding(string const& encoding)
{
}
