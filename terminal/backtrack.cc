#include "terminal/backtrack.h"

#include "terminal/charmatrix.h"

Backtrack::Backtrack(CharMatrix const& charmatrix, int n_screens)
	: charmatrix(charmatrix), n_screens(n_screens)
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
	unsigned int n = (charmatrix.H() - y - 1) + (charmatrix.H() * (backscreen - 1));
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
	return charmatrix.H() * (n_screens);
}
