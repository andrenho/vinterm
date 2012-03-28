#ifndef CHARS_H
#define CHARS_H

#include "SDL.h"
#include <stdint.h>
#include <map>
using namespace std;

#include "font.h"
#include "options.h"
#include "terminal.h"

class Chars
{
public:
	Chars(Options const& options, Font const& font);
	~Chars();

	SDL_Surface* Char(const uint8_t c, const CharAttr attr) const;

	const int start_at_x, start_at_y;

private:
	SDL_Surface* CreateChar(const uint8_t c, const CharAttr attr) const;
	inline uint16_t AttrIndex(const uint8_t c, const CharAttr attr) const { return (int(attr) << 8) + c; }

	Options const& options;
	Font const& font;
	mutable map<uint16_t, SDL_Surface*> chars;
	SDL_Surface** reverse_space;
};

#endif
