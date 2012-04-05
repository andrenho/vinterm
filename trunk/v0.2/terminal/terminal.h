#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>
#include <sstream>
using namespace std;

#include "terminal/pty.h"

class Framebuffer;
class Screen;

class Terminal
{
public:
	Terminal(Framebuffer& fb, PTY& pty);
	~Terminal();

	bool Active() const { return active; }
	void Input();
	void Output(Screen const& screen);

protected:
	Framebuffer& fb;
	PTY& pty;
	bool active;
};

#endif
