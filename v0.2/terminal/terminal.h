#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>
using namespace std;

#include "terminal/pty.h"

class Framebuffer;
class GraphicLibrary;

class Terminal
{
public:
	Terminal(Framebuffer& fb, GraphicLibrary const& gl, PTY const& pty);
	~Terminal();

	bool Active() const { return active; }
	void Input();
	void Output();

protected:
	Framebuffer& fb;
	GraphicLibrary const& gl;
	PTY const& pty;
	bool active;
};

#endif