#include "graphic/chars.h"

#include "options.h"
#include "filters/filter.h"
#include "graphic/font.h"
#include "graphic/screen.h"

Chars::Chars(Options const& options, Font const& font)
	: start_at_x(1), start_at_y(1),
	  options(options),
	  font(font),
	  reverse_space(new SDL_Surface*[10])
{
	// see Char::Chars for an explaination
	Attribute attr;
	attr.Reverse = true;
	for(int i=0; i<10; i++)
		reverse_space[i] = CreateChar(' ', attr);
}


Chars::~Chars()
{
	for(int i(0); i<10; i++)
		SDL_FreeSurface(reverse_space[i]);
	delete[] reverse_space;

	for(map<uint16_t, SDL_Surface*>::iterator it(chars.begin()); 
			it != chars.end(); it++)
		SDL_FreeSurface(it->second);
	chars.clear();
}


SDL_Surface* 
Chars::CreateChar(const uint8_t c, const Attribute attr) const
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
	int color(160);
	if(attr.Highlight)
		color = 255;
	else if(attr.Dim)
		color = 110;
	if(attr.Reverse)
	{
		int t(bg_color);
		bg_color = color;
		color = t;
	}

	// copy char
	SDL_FillRect(s, NULL, bg_color);
	if(options.scale == 1)
	{
		for(int x(0); x<font.char_w; x++)
			for(int y(0); y<font.char_h; y++)
				P(s, x + start_at_x, y + start_at_y) = 
					font.ch[c][y*font.char_w+x] ? color : bg_color;
		if(attr.Underline)
		{
			SDL_Rect r = { 0, (Sint16)font.char_h, 
				(Sint16)font.char_w, 1 };
			SDL_FillRect(s, &r, color);
		}
	}
	else
	{
		for(int x(0); x<font.char_w; x++)
			for(int y(0); y<font.char_h; y++)
			{
				SDL_Rect r = {
					(Sint16)((x + start_at_x) * options.scale),
					(Sint16)((y + start_at_y) * options.scale),
					(Sint16)options.scale, 
					(Sint16)options.scale };
				if(font.ch[c][y*font.char_w+x])
					SDL_FillRect(s, &r, color);
			}
		if(attr.Underline)
		{
			SDL_Rect r = { 0, 
				(Sint16)(font.char_h * options.scale), 
				(Sint16)(font.char_w * options.scale), 
				(Sint16)options.scale };
			SDL_FillRect(s, &r, color);
		}
	}

	// apply filters
	vector<Filter*>::const_iterator filter;
	for(filter = options.PreFilters()->begin(); 
			filter < options.PreFilters()->end(); 
			filter++)
		(*filter)->Apply(s, options);

	// remove borders from the reverse - avoid artifacts
	if(attr.Reverse)
	{
		const int S(options.scale);
		SDL_Rect r[4] = {
			{ 0, 0, (Sint16)(start_at_x*S), (Sint16)s->h },
			{ 0, 0, (Sint16)s->w, (Sint16)(start_at_y*S) },
			{ (Sint16)(s->w - (start_at_x*S)), 0, 
				(Sint16)(start_at_x*S), (Sint16)s->h },
			{ 0, (Sint16)(s->h - (start_at_y*S)), 
				(Sint16)s->w, (Sint16)(start_at_y*S) }
		};
		for(int i(0); i<4; i++)
			SDL_FillRect(s, &r[i], 0);
	}

	return s;
}


SDL_Surface* 
Chars::Char(const uint8_t c, const Attribute attr, int rnd) const 
{
	if(c == ' ' && attr.Reverse)
	{
		// here we need to make a little gimmick and each time return
		// a different image to avoid artifacts on the screen
		srand(rnd);
		return reverse_space[rand() % 10];
	}
	else
	{
		uint16_t n = AttrIndex(c, attr);
		if(!chars.count(n))
			chars[n] = CreateChar(c, attr);
		return chars[n];
	}
}
