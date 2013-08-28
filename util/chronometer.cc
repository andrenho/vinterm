#include "util/chronometer.h"

#include <iostream>
using namespace std;

#include "SDL2/SDL_timer.h"

Chronometer::Chronometer()
	: current_stage(""), last_time(0)
{
}


Chronometer::~Chronometer()
{
}


void 
Chronometer::Next(string stage)
{
	uint32_t now = SDL_GetTicks();
	if(current_stage != "")
	{
		if(times.find(current_stage) == times.end())
			times[current_stage] = 0;
		times[current_stage] += (now - last_time);
	}
	last_time = now;
	current_stage = stage;
}


void 
Chronometer::Report()
{
	cout << endl;

	uint32_t sum = 0;
	for(auto& kv : times)
		sum += kv.second;
	for(auto& kv : times)
		cout << kv.first << ": " << ((double)kv.second) / sum << endl;
}
