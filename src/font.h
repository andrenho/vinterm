#ifndef FONT_H
#define FONT_H

#include <stdint.h>

class Font
{
public:
	Font();
	~Font();

	const int char_w, char_h;
	uint8_t** const ch;
};

#endif
