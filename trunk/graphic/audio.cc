#include "audio.h"

#include <stdio.h>
#include <ao/ao.h>
#include <math.h>
#include <string.h>

#define BUF_SIZE 4096

#include <iostream>
using namespace std;

Audio::Audio()
{
	int default_driver;

	/* -- Initialize -- */

	fprintf(stderr, "libao example program\n");

	ao_initialize();

	/* -- Setup for default driver -- */

	default_driver = ao_default_driver_id();

        memset(&format, 0, sizeof(format));
	format.bits = 16;
	format.channels = 2;
	format.rate = 44100;
	format.byte_format = AO_FMT_LITTLE;

	/* -- Open driver -- */
	device = ao_open_live(default_driver, &format, NULL /* no options */);
	if (device == NULL) {
		fprintf(stderr, "Error opening device.\n");
		return;
	}
}


Audio::~Audio()
{
	/* -- Close and shutdown -- */
	ao_close(device);

	ao_shutdown();
}


void 
Audio::Beep()
{
	char *buffer;
	int buf_size;
	int sample;
	float freq = 440.0;
	int i;
	float length = 0.05;

	/* -- Play some stuff -- */
	buf_size = format.bits/8 * format.channels * format.rate * length;
	buffer = (char*)calloc(buf_size,
			sizeof(char));

	for (i = 0; i < format.rate*length; i++) {
		sample = (int)(0.75 * 32768.0 *
			sin(2 * M_PI * freq * ((float) i/format.rate)));

		/* Put the same stuff in left and right channel */
		buffer[4*i] = buffer[4*i+2] = sample & 0xff;
		buffer[4*i+1] = buffer[4*i+3] = (sample >> 8) & 0xff;
	}
	ao_play(device, buffer, buf_size);
}
