#include <string>
#include <iostream>
using namespace std;

#include "options.h"
#include "graphic/curses_lib.h"
#include "terminal/framebuffer.h"
#include "terminal/terminal.h"

int main(int argc, char** argv)
{
	// initialize graphic library
	GraphicLibrary* gl;
	try 
	{
		gl = GraphicLibrary::Initialize(GraphicLibrary::CURSES, 80, 24);
	} 
	catch(string& er) 
	{
		cerr << er << endl;
		return 1;
	}

	Options options(argc, argv);
	Framebuffer fb(options);
	PTY pty;
	gl->Initialize(options, fb);
	Terminal terminal(fb, *gl, pty);

	while(terminal.Active())
	{
		terminal.Input();
		terminal.Output();
		gl->Update();
	}

	delete gl;

	return 0;
}
