#ifndef VINTERM_H
#define VINTERM_H

#include "terminal/terminal.h"

class Vinterm : public Terminal
{
public:
	Vinterm(Framebuffer const& fb, GraphicLibrary const& gl)
		: Terminal(fb, gl) { }
};

#endif
