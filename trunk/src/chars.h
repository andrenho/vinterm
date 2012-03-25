#ifndef CHARS_H
#define CHARS_H

#include "SDL.h"
#include <stdint.h>

#include "font.h"
#include "options.h"
#include "terminal.h"

class Chars
{
public:
	Chars(Options const& options, Font const& font);
	~Chars();

	SDL_Surface* Char(uint8_t c, CharAttr attr) const;

	const int start_at_x, start_at_y;

private:
	SDL_Surface* CreateChar(uint8_t c, CharAttr attr);

	Options const& options;
	Font const& font;
	SDL_Surface*** chars;
	SDL_Surface** reverse_space;
};

#endif
