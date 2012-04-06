#include "terminal/vinterm.h"

#include <cctype>

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
	case 'B': // move cursor down
		fb.MoveCursor(Direction::DOWN, p[0]);
		break;
	case 'C': // move cursor forward
		fb.MoveCursor(Direction::RIGHT, p[0]);
		break;
	case 'D': // move cursor forward
		fb.MoveCursor(Direction::LEFT, p[0]);
		break;
	case 'H': // move cursor home
		fb.Home();
		break;
	case 'L':
		fb.LowerLeft();
		break;
	default:
		Terminal::ExecuteEscapeSequence(sequence);
	}
}


char 
Vinterm::ParseEscapeSequence(string const& sequence, vector<int>& params)
{
	char cmd = sequence[sequence.length()-1];

	for(string::const_iterator c = sequence.begin();
			c < sequence.end(); c++)
	{
		string s;
		while(isdigit(*c))
		{
			s += *c;
			c++;
		}
		if(!s.empty())
			params.push_back(strtol(s.c_str(), NULL, 10));
	}

	return cmd;
}
