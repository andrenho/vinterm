#ifndef CHARS_H
#define CHARS_H

#include "SDL.h"
#include <stdint.h>
#include <map>
using namespace std;

#include "terminal/charattr.h"

class Options;
class Font;

class Chars
{
public:
	Chars(Options const& options, Font const& font);
	~Chars();

	SDL_Surface* Char(const uint8_t c, const Attribute attr,
			int rnd=0) const;

	const int start_at_x, start_at_y;

private:
	SDL_Surface* CreateChar(const uint8_t c, const Attribute attr) const;
	inline uint16_t AttrIndex(const uint8_t c, const Attribute attr) const { return (int(attr) << 8) + c; }

	Options const& options;
	Font const& font;
	mutable map<uint16_t, SDL_Surface*> chars;
	SDL_Surface** reverse_space;
};

#endif
