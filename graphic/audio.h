#ifndef AUDIO_H
#define AUDIO_H

#include "SDL.h"

class Audio
{
public:
	Audio();
	~Audio();

	void Beep();

private:
	Uint8* buf;
};

#endif
