#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

class Options;

class Framebuffer
{
public:
	Framebuffer(Options const& options) : options(options) { }

private:
	Options const& options;
};

#endif
