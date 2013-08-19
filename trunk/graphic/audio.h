#ifndef AUDIO_H
#define AUDIO_H

#ifdef AO
#  include <ao/ao.h>
#endif

class Options;

class Audio
{
public:
	Audio(Options const& options);
	~Audio();

	void Beep() const;

private:
	bool active;
#ifdef AO
	ao_device *device;
	ao_sample_format format;
#endif
};

#endif
