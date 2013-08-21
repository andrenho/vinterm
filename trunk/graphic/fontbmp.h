#ifndef FONTBMP_H
#define FONTBMP_H

#include <string>
using namespace std;

#include "graphic/font.h"

class FontBMP : public Font
{
public:
	FontBMP(string filename, int w, int h, int chars_per_row, 
			string encoding);
	~FontBMP();

	void DrawChar(uint32_t ch, Framebuffer& fb, int x, int y) const;
	string Encoding() const { return encoding; }
	int CharWidth() const { return w+1; }
	int CharHeight() const { return h+1; }

private:
	const int w, h;
	const string encoding;
};

#endif
