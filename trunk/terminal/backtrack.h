#ifndef BACKTRACK_H
#define BACKTRACK_H

#include <deque>
#include <vector>
using namespace std;

#include "terminal/charattr.h"
class Framebuffer;

class Backtrack
{
public:
	Backtrack(Framebuffer const& framebuffer, int n_screens);
	~Backtrack();

	void PushLine(vector<Char>& line);
	Char Get(int backscreen, unsigned int x, int y) const;

	inline int Screens() const { return n_screens; }

private:
	void PrepareBacktrack();
	unsigned int TotalLines() const;

	Framebuffer const& framebuffer;
	int n_screens;
	deque<vector<Char>> lines;
};

#endif
