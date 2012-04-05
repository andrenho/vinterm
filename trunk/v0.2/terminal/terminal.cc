#include "terminal/terminal.h"

using namespace std;

#include "terminal/framebuffer.h"
#include "terminal/pty.h"
#include "graphic/graphiclibrary.h"

Terminal::Terminal(Framebuffer& fb, GraphicLibrary const& gl, PTY& pty)
	: fb(fb), gl(gl), pty(pty), active(true)
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
		fb.Put(c);
	}
}


void 
Terminal::Output()
{
	// write data to the PTY
	int ch = gl.Input();

	if(ch)
		pty.Send((const char)ch);
}
