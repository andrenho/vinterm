#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdint.h>
#include <vector>
#include <string>
using namespace std;

class Options
{
public:
	Options(const int argc, char** const argv);
	~Options();

	int scale;
	const int w, h;
	bool debug_terminal;
	const int border_x, border_y;
	const uint32_t blink_speed, flashing_speed;
	bool audio_disabled;
	string toBeRun;
	string CurrentEncoding;

private:
	void ReadConfigFile();
	void WriteConfigFile();
	void ParseArguments(int argc, char* argv[]);
	void Help(int status);
	void Version();
};

#endif
