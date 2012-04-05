#include "terminal/terminal.h"

#include <sstream>
using namespace std;

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
	stringstream s;
	s << pty;

	for(string::const_iterator it = s.str().begin(); it < s.str().end(); 
			it++)
		'x' >> fb;
}


void 
Terminal::Output()
{
}
