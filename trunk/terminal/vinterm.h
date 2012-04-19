#ifndef VINTERM_H
#define VINTERM_H

#include <vector>
using namespace std;

#include "terminal/terminal.h"

class Vinterm : public Terminal
{
public:
	Vinterm(Framebuffer& fb, PTY& pty, Options const& options) 
		: Terminal(fb, pty, options), saved_x(0), saved_y(0) { }
	
	virtual void ExecuteEscapeSequence(string const& sequence);
	void KeyPressed(int key);

private:
	char ParseEscapeSequence(string const& sequence, vector<int>& params);
	int saved_x, saved_y;
};

#endif
