#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <cstdint>

#include "global.h"

class Options;
class Font;

struct Color { uint8_t r, g, b; };

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	void Resize(int w, int h, int ts_w, int ts_h);
	void DrawChars();

	inline const uint8_t* const Pixels() const { return pixels; }

	Color palette[255];
private:
	int w, h;
	uint8_t* pixels;
};

#endif
