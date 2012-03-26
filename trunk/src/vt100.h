#ifndef VT100_H
#define VT100_H

#include "terminal.h"

class VT100 : public Terminal
{
public:
	VT100(Options const& options)
		: Terminal(options, "vt100") { }
};

#endif
