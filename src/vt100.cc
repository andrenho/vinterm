#include "vt100.h"

#include <iostream>
using namespace std;

void 
VT100::ExecuteEscapeSequence(string const& s)
{
	char command;
	vector<string> parameters;
	ParseCommand(s, command, parameters);

	switch(command)
	{
	default:
		Terminal::ExecuteEscapeSequence(s);
	}
}


void 
VT100::ParseCommand(string const& seq, char& command, vector<string> parameters)
{
	
}
