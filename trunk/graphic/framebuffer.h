#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <cstdint>
#include <vector>
using namespace std;

#include "global.h"

class Options;
class Font;

struct Color { uint8_t r, g, b; };
struct Rectangle {
	int x, y, w, h;
	Rectangle(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) { }
};

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	void Resize(int w, int h, int ts_w, int ts_h);
	void DrawChars();

	inline const uint8_t* const Pixels() const { return pixels; }

	Color palette[255];
	vector<Rectangle> rects;

	int W() const { return w; }
	int H() const { return h; }
	
private:
	int w, h;
	uint8_t* pixels;
};

#endif
