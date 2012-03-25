#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdint.h>
#include <vector>
#include "SDL.h"
using namespace std;

class Filter;

class Options
{
public:
	Options(int argc, char* argv[]);
	~Options();

	inline const vector<Filter*>* PreFilters() const { return &prefilters; }

	int scale;
	const int w, h;
	const int border_x, border_y;
	const SDL_Color background_color, bright_color;
	const uint32_t blink_speed;

private:
	void ParseArguments(int argc, char* argv[]);
	void AddFilters();
	void Help(int status);
	void Version();

	vector<Filter*> prefilters;
};

#endif
