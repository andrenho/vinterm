#include "options.h"

#include <getopt.h>
#include <locale.h>
#include <langinfo.h>
#include <iostream>
using namespace std;

#include "filters/filter_inexact.h"
#include "filters/filter_scanline.h"
#include "filters/filter_bright.h"

Options::Options(const int argc, char** const argv) 
	: scale(1), w(80), h(25), debug_terminal(false), 
	  border_x(30), border_y(40),
	  background_color(SDL_Color { 30, 30, 30 }),
	  bright_color(SDL_Color { 140, 255, 190 }),
	  blink_speed(500), flashing_speed(100),
	  audio_active(true)
{
	ParseArguments(argc, argv);
	AddFilters();

	// find out current encoding
	char* loc = setlocale(LC_ALL, "");

	if(loc)
		CurrentEncoding = nl_langinfo(_NL_CTYPE_CODESET_NAME);
	else
	{
		cerr << "warning: the current locale could not be identified."
			<< endl;
		CurrentEncoding = string();
	}
}

void
Options::ParseArguments(int argc, char* argv[])
{
	int c;
	while(1)
	{
		int idx(0);
		static struct option long_opt[] = {
			{ "scale",    required_argument, 0, 's' },
			{ "no-audio", no_argument,       0, 'a' },
#ifdef DEBUG
			{ "debug-terminal", no_argument, 0, '@' },
#endif
			{ "version",  no_argument,       0, 'v' },
			{ "help",     no_argument,       0, '?' },
			{ 0,          0,                 0, 0 }
		};

		if((c = getopt_long(argc, argv, "s:a@", long_opt, &idx)) == -1)
			break;

		if(c == -1)
			break;
		
		switch(c)
		{
		case 0:
			if(strcmp(long_opt[idx].name, "help") == 0)
				Help(EXIT_SUCCESS);
			else if(strcmp(long_opt[idx].name, "version") == 0)
				Version();
			else
				Help(EXIT_FAILURE);
		case 's':
			scale = strtol(optarg, NULL, 10);
			if(scale < 1 || scale > 6)
			{
				fprintf(stderr, "error: Scale must be a value between 1 and 6.\n");
				exit(1);
			}
			break;
		case 'a':
			audio_active = false;
			break;
		case '@':
#ifdef DEBUG
			debug_terminal = true;
#else
			fprintf(stderr, "vinterm wasn't compiled with debugging	information.\n");
			exit(1);
#endif
			break;
		case 'v':
			Version();
			break;
		case '?':
			Help(EXIT_FAILURE);
			break;

		default:
			abort();
		}
	}

	// remaining commandline arguments
	if(optind < argc)
	{
		while(optind < argc)
		{
			toBeRun += argv[optind++];
			toBeRun += " ";
		}
		toBeRun += "\n";
	}
}


void
Options::AddFilters()
{
	if(scale == 1)
	{
		prefilters.push_back(new FilterBright(3,10));
		prefilters.push_back(new FilterScanline(0.9));
		prefilters.push_back(new FilterInexact(15));
	}
	else
	{
		prefilters.push_back(new FilterBright(2,0));
		prefilters.push_back(new FilterScanline());
		prefilters.push_back(new FilterInexact(30));
	}
}


Options::~Options()
{
	vector<Filter*>::reverse_iterator filter;
	for(filter = prefilters.rbegin(); filter < prefilters.rend(); filter++)
		delete (*filter);
	prefilters.clear();
}


void 
Options::Help(int status)
{
	FILE* f(stdout);
	if(status == EXIT_FAILURE)
		f = stderr;

	fprintf(f, "Usage: vinterm [OPTIONS] [COMMAND]\n");
	fprintf(f, "`Vintage Terminal` is a terminal emulator that simulates the look of a old\n");
	fprintf(f, "monitor.\n");
	fprintf(f, "\n");
	fprintf(f, "Options:\n");
	fprintf(f, "  -s, --scale=SCALE    zoom the image by SCALE (default=1)\n");
	fprintf(f, "  -a, --no-audio       disable audible beep\n");
	fprintf(f, "      --help           display this help and exit\n");
	fprintf(f, "      --version        display version information and exit\n");
	fprintf(f, "\n");
	fprintf(f, "COMMAND must be the last argument, and sent directly to the terminal on\n");
	fprintf(f, "initialization, along with a new line.\n");
	fprintf(f, "\n");
	fprintf(f, "Keys:\n");
	fprintf(f, "  CTRL+F11             full screen\n");
	fprintf(f, "  CTRL+SHIFT+F11       full screen with 80 columns\n");
	fprintf(f, "\n");
	fprintf(f, "Report bugs to: <http://code.google.com/p/vinterm/issues/>\n");
	fprintf(f, "`Vintage Terminal` home page: <http://code.google.com/p/vinterm/>\n");

	exit(status);
}


void 
Options::Version()
{
	printf("`Vintage Terminal` (vinterm) " VERSION "\n");
	printf("Copyright (C) 2012-2013 André Wagner\n");
	printf("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n");
	printf("This is free software: you are free to change and redistribute it.\n");
	printf("There is NO WARRANTY, to the extent permitted by law.\n");

	exit(EXIT_SUCCESS);
}
