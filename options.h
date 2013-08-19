#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdint.h>
#include <vector>
#include <string>
#include "SDL.h"
using namespace std;

class Filter;

class Options
{
public:
	Options(const int argc, char** const argv);
	~Options();

	inline const vector<Filter*>* PreFilters() const { return &prefilters; }

	int scale;
	const int w, h;
	bool debug_terminal;
	const int border_x, border_y;
	const SDL_Color background_color, bright_color;
	const uint32_t blink_speed, flashing_speed;
	bool audio_active;
	string toBeRun;
	string CurrentEncoding;

private:
	void ReadConfigFile();
	void WriteConfigFile();
	void ParseArguments(int argc, char* argv[]);
	void AddFilters();
	void Help(int status);
	void Version();

	vector<Filter*> prefilters;
};

#endif
