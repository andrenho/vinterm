#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <cstdint>

class Options;
class CharMatrix;

struct Color { uint8_t r, g, b; };

class Framebuffer
{
public:
	Framebuffer(Options const& options, CharMatrix const& cm);
	~Framebuffer();

	Color palette[255];

	void Resize(int w, int h);

private:
	Options const& options;
	CharMatrix const& cm;
	int w, h;
	uint8_t* pixels;
};

#endif
