#include "terminal/backtrack.h"

#include "terminal/framebuffer.h"

Backtrack::Backtrack(Framebuffer const& framebuffer, int n_screens)
	: framebuffer(framebuffer), n_screens(n_screens)
{
	PrepareBacktrack();
}


Backtrack::~Backtrack()
{
	lines.clear();
}


void 
Backtrack::PushLine(vector<Char>& line)
{
	lines.push_front(line);
	while(lines.size() > TotalLines())
		lines.pop_back();
}


Char 
Backtrack::Get(int backscreen, unsigned int x, int y) const
{
	unsigned int n = (framebuffer.H() - y - 1) + (framebuffer.H() * (backscreen - 1));
	if(n >= lines.size())
		return Char();
	vector<Char> const& l = lines[n];
	if(x >= l.size())
		return Char();
	return l[x];
}


void 
Backtrack::PrepareBacktrack()
{
	lines.clear();
}


unsigned int
Backtrack::TotalLines() const
{
	return framebuffer.H() * (n_screens);
}
