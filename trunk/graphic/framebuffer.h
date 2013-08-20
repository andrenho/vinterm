#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <cstdint>

class Options;
class CharMatrix;

class Framebuffer
{
public:
	Framebuffer(Options const& options, CharMatrix const& cm);
	~Framebuffer();

	void Resize(int w, int h);

private:
	Options const& options;
	CharMatrix const& cm;
	int w, h;
	uint32_t* pixels;
};

#endif
