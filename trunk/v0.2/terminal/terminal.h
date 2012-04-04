#ifndef TERMINAL_H
#define TERMINAL_H

class Framebuffer;
class GraphicLibrary;

class Terminal
{
public:
	Terminal(Framebuffer const& fb, GraphicLibrary const& gl)
		: fb(fb), gl(gl) { }

	bool Active() { return false; }
	void Input() { }
	void Output() { }

protected:
	Framebuffer const& fb;
	GraphicLibrary const& gl;
};

#endif
