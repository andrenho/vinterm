#include "terminal/terminal.h"

using namespace std;

#include "terminal/framebuffer.h"
#include "terminal/pty.h"

Terminal::Terminal(Framebuffer& fb, GraphicLibrary const& gl,
		PTY const& pty)
	: fb(fb), gl(gl), pty(pty), active(true)
{
}


Terminal::~Terminal()
{
}


void
Terminal::Input()
{
	int i;

	while((i = pty.Get()) != PTY::NO_DATA)
	{
		const char c = (const char)i;
		fb.Put(c);
	}
}


void 
Terminal::Output()
{
}
