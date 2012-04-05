#include <string>
#include <iostream>
using namespace std;

#include "options.h"
#include "graphic/screen.h"
#include "terminal/framebuffer.h"
#include "terminal/terminal.h"

int main(int argc, char** argv)
{
	Options options(argc, argv);
	Framebuffer fb(options);
	PTY pty;
	Terminal terminal(fb, pty);
	Screen screen(options, fb);

	while(terminal.Active())
	{
		terminal.Input();
		terminal.Output(screen);
		screen.Update();
	}

	return 0;
}
