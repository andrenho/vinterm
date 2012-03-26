#ifndef VT100_H
#define VT100_H

#include <vector>
#include <string>
using namespace std;

#include "terminal.h"

class VT100 : public Terminal
{
public:
	VT100(Options const& options)
		: Terminal(options, "vt100") { }

protected:
	virtual void ExecuteEscapeSequence(string const& s);

private:
	void ParseCommand(string const& seq, char& command, 
			vector<string> parameters);
};

#endif
