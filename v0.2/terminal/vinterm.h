#ifndef VINTERM_H
#define VINTERM_H

#include <vector>
using namespace std;

#include "terminal/terminal.h"

class Vinterm : public Terminal
{
public:
	Vinterm(Framebuffer& fb, PTY& pty) : Terminal(fb, pty) { }
	
	virtual void ExecuteEscapeSequence(string const& sequence);

private:
	char ParseEscapeSequence(string const& sequence, vector<int>& params);
};

#endif
