#ifndef CHRONOMETER_H
#define CHRONOMETER_H

#include <cstdint>
#include <string>
#include <map>
using namespace std;

class Chronometer
{
public:
	Chronometer();
	~Chronometer();

	void Next(string stage);
	void Report();

private:
	map<string, uint32_t> times;
	string current_stage;
	uint32_t last_time;
};

#endif
