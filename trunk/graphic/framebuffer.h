#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <cstdint>

class Options;
class CharMatrix;
class Font;

struct Color { uint8_t r, g, b; };

class Framebuffer
{
public:
	Framebuffer(Options const& options, CharMatrix const& cm, 
			Font const& font);
	~Framebuffer();

	void Resize(int w, int h);
	void DrawChars();

	const uint8_t* const Pixels() const { return pixels; }

	Color palette[255];
private:
	Options const& options;
	CharMatrix const& cm;
	Font const& font;
	int w, h;
	uint8_t* pixels;
};

#endif
