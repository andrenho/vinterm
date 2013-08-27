#ifndef VINTERM_H
#define VINTERM_H

#include <vector>
using namespace std;

#include "terminal/terminal.h"

class Vinterm : public Terminal
{
public:
	Vinterm();
	
	virtual void ExecuteEscapeSequence(string const& sequence);
	virtual void InputAlternateChar(const char c);
	void KeyPressed(uint32_t key);

private:
	char ParseEscapeSequence(string const& sequence, vector<int>& params);
	int saved_x, saved_y;
};

#endif
