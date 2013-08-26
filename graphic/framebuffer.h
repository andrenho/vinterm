#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <cstdint>

#include "terminal/charmatrix.h"

class Options;
class Font;

struct Color { uint8_t r, g, b; };

class Framebuffer
{
public:
	Framebuffer(Options const& options, CharMatrix& cm, Font const& font);
	~Framebuffer();

	void Resize(int w, int h, int ts_w, int ts_h);
	void DrawChars();

	inline const uint8_t* const Pixels() const { return pixels; }
	inline string TerminalTitle() const { return cm.TerminalTitle(); }

	Color palette[255];
private:
	Options const& options;
	CharMatrix& cm;
	Font const& font;
	int w, h;
	uint8_t* pixels;
};

#endif
