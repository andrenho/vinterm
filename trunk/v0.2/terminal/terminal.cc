#include "terminal/terminal.h"

using namespace std;

#include "terminal/framebuffer.h"
#include "terminal/pty.h"
#include "graphic/screen.h"

Terminal::Terminal(Framebuffer& fb, PTY& pty)
	: fb(fb), pty(pty), active(true)
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
		const char c = (const char)i;
		switch(c)
		{
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
