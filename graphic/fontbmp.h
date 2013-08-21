#ifndef FONTBMP_H
#define FONTBMP_H

#include <string>
using namespace std;

#include "SDL2/SDL.h"

#include "graphic/font.h"

class FontBMP : public Font
{
public:
	FontBMP(string filename, int w, int h, int chars_per_row, 
			string encoding);
	~FontBMP();

	void DrawChar(uint32_t ch, uint8_t* pixels, int x, int y, int pitch,
			uint8_t bg_color, uint8_t fg_color) const;
	string Encoding() const { return encoding; }
	int CharWidth() const { return w; }
	int CharHeight() const { return h; }

private:
	void FindColors();

	const int w, h, chars_per_row;
	const string encoding;
	SDL_Surface* sf;
	uint8_t background, foreground;
};

#endif
