#ifndef VINTERM_H
#define VINTERM_H

#include "terminal.h"

class Vinterm : public Terminal
{
public:
	explicit Vinterm(Options const& options)
		: Terminal(options, "vinterm") { }
	
};

#endif
