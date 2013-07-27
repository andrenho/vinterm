#ifndef AUDIO_H
#define AUDIO_H

#include <ao/ao.h>

class Audio
{
public:
	Audio();
	~Audio();

	void Beep();

private:
	ao_device *device;
	ao_sample_format format;
};

#endif
