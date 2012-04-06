#include "terminal/vinterm.h"

#include "terminal/framebuffer.h"

void 
Vinterm::ExecuteEscapeSequence(string const& sequence)
{
	vector<int> p;
	char command = ParseEscapeSequence(sequence, p);

	if(p.size() <= 2)
	{
		p.push_back(0);
		p.push_back(0);
	}

	switch(command)
	{
	case 'A': // move cursor up
		fb.MoveCursor(Direction::UP, p[0]);
		break;
	default:
		Terminal::ExecuteEscapeSequence(sequence);
	}
}


char 
Vinterm::ParseEscapeSequence(string const& sequence, vector<int>& params)
{
	char cmd = sequence[sequence.length()-1];
	return cmd;
}
