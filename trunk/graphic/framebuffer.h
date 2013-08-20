#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

class Options;
class CharMatrix;

class Framebuffer
{
public:
	Framebuffer(Options const& options, CharMatrix const& cm);
	~Framebuffer();

private:
	Options const& options;
	CharMatrix const& cm;
};

#endif
