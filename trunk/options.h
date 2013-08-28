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

	bool debug_terminal;
	bool audio_disabled;
	bool fullscreen;
	string renderer;
	string toBeRun;
	string CurrentEncoding;
	const uint32_t blink_speed, flashing_speed;

private:
	void ReadConfigFile();
	void WriteConfigFile();
	void ParseArguments(int argc, char* argv[]);
	void Help(int status);
	void Version();
};

#endif
