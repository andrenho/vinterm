#include "chars.h"

#include "screen.h"
#include "filter.h"

Chars::Chars(Options const& options, Font const& font)
	: start_at_x(1), start_at_y(1),
	  options(options),
	  font(font),
	  chars(new SDL_Surface**[256]),
	  reverse_space(new SDL_Surface*[10])
{
	for(uint8_t i(0); i<255; i++)
	{
		chars[i] = new SDL_Surface*[NUM_ATTRS];
		chars[i][NORMAL] = CreateChar(i, NORMAL);
		chars[i][REVERSE] = CreateChar(i, REVERSE);
	}

	// see Char::Chars for an explaination
	for(int i=0; i<10; i++)
		reverse_space[i] = CreateChar(' ', REVERSE);
}


Chars::~Chars()
{
	for(int i(0); i<10; i++)
		SDL_FreeSurface(reverse_space[i]);
	delete[] reverse_space;
	for(uint8_t i(0); i<255; i++)
	{
		for(int j(0); j<NUM_ATTRS; j++)
			SDL_FreeSurface(chars[i][j]);
		delete[] chars[i];
	}
	delete[] chars;
}


SDL_Surface* 
Chars::CreateChar(const uint8_t c, const CharAttr attr)
{
	// create surface
	SDL_Surface* s(SDL_CreateRGBSurface(SDL_SWSURFACE,
			(font.char_w+(2*start_at_x)) * options.scale, 
			(font.char_h+(2*start_at_y)) * options.scale,
			8, 0, 0, 0, 0));
	Screen::initializePalette(s, options);
	SDL_SetColorKey(s, SDL_SRCCOLORKEY, 0);

	// setup colors
	int bg_color(0);
	int color(200); // TODO
	if(attr == REVERSE)
	{
		int t(bg_color);
		bg_color = color;
		color = t;
	}

	// copy char
	SDL_FillRect(s, NULL, bg_color);
	if(options.scale == 1)
		for(int x(0); x<font.char_w; x++)
			for(int y(0); y<font.char_h; y++)
				P(s, x + start_at_x, y + start_at_y) = 
					font.ch[c][y*font.char_w+x] ? color : bg_color;
	else
		for(int x(0); x<font.char_w; x++)
			for(int y(0); y<font.char_h; y++)
			{
				SDL_Rect r = {
					(x + start_at_x) * options.scale,
					(y + start_at_y) * options.scale,
					options.scale, options.scale };
				if(font.ch[c][y*font.char_w+x])
					SDL_FillRect(s, &r, color);
			}

	// apply filters
	vector<Filter*>::const_iterator filter;
	for(filter = options.PreFilters()->begin(); 
			filter < options.PreFilters()->end(); 
			filter++)
		(*filter)->Apply(s, options);

	// remove borders from the reverse - avoid artifacts
	if(attr == REVERSE)
	{
		const int S(options.scale);
		SDL_Rect r[4] = {
			{ 0, 0, start_at_x*S, s->h },
			{ 0, 0, s->w, start_at_y*S },
			{ s->w - (start_at_x*S), 0, start_at_x*S, s->h },
			{ 0, s->h - (start_at_y*S), s->w, start_at_y*S }
		};
		for(int i(0); i<4; i++)
			SDL_FillRect(s, &r[i], 0);
	}

	return s;
}


SDL_Surface* 
Chars::Char(const uint8_t c, const CharAttr attr) const 
{
	if(c == ' ' && attr == REVERSE)
	{
		// here we need to make a little gimmick and each time return
		// a different image to avoid artifacts on the screen
		return reverse_space[rand() % 10];
	}
	else
		return chars[c][attr]; 
}
