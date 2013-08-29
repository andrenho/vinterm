#include "options.h"

#include <cstring>

#include <getopt.h>
#include <locale.h>
#include <langinfo.h>
#include <iostream>
using namespace std;

#include <libconfig.h++>
using namespace libconfig;

Options::Options(const int argc, char** const argv) 
	: debug_terminal(false), audio_disabled(false), fullscreen(false),
	  renderer("monochrome"), blink_speed(500), flashing_speed(100)
{
	ReadConfigFile();
	ParseArguments(argc, argv);

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
Options::ReadConfigFile()
{
	// open and parse file
	Config cfg;
	try 
	{
		char filename[1024];
		snprintf(filename, 1023, "%s/.vinterm.conf", getenv("HOME"));
		cfg.readFile(filename);
	}
	catch(FileIOException& e)
	{
		cerr << "warning: config file not found, creating one." << endl;
		WriteConfigFile();
		return;
	}
	catch(ParseException& e)
	{
		cerr << "error: invalid syntax on config file, ignoring... (" << 
			e.getError() << ")" << endl;
		return;
	}

	// read data
	cfg.lookupValue("no_audio", audio_disabled);
	cfg.lookupValue("fullscreen", fullscreen);
	cfg.lookupValue("renderer", renderer);
}


void
Options::WriteConfigFile()
{
	char filename[1024];
	snprintf(filename, 1023, "%s/.vinterm.conf", getenv("HOME"));
	FILE* f = fopen(filename, "w");
	if(!f) {
		cerr << "error: it wasn't possible to write config file:";
		perror(NULL); 
		cerr << endl;
		return;
	}

	fprintf(f, "# graphic renderer (monochrome, simple)\n");
	fprintf(f, "fullscreen = %s\n", renderer.c_str());
	fprintf(f, "\n");
	fprintf(f, "# disable audible beep\n");
	fprintf(f, "no_audio = %s\n", audio_disabled ? "true" : "false");
	fprintf(f, "\n");
	fprintf(f, "# initialize in full screen\n");
	fprintf(f, "fullscreen = %s\n", fullscreen ? "true" : "false");

	fclose(f);
}


void
Options::ParseArguments(int argc, char* argv[])
{
	int c;
	while(1)
	{
		int idx(0);
		static struct option long_opt[] = {
			{ "no-audio", no_argument,       0, 'a' },
			{ "fullscreen", no_argument,     0, 'f' },
			{ "renderer", required_argument, 0, 'r' },
#ifdef DEBUG
			{ "debug-terminal", no_argument, 0, '@' },
#endif
			{ "version",  no_argument,       0, 'v' },
			{ "help",     no_argument,       0, '?' },
			{ 0,          0,                 0, 0 }
		};

		if((c = getopt_long(argc, argv, "r:af@", long_opt, &idx)) == -1)
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
			break;
		case 'a':
			audio_disabled = true;
			break;
		case 'f':
			fullscreen = true;
			break;
		case '@':
#ifdef DEBUG
			debug_terminal = true;
#else
			fprintf(stderr, "vinterm wasn't compiled with debugging	information.\n");
			exit(1);
#endif
			break;
		case 'r':
			renderer = optarg;
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


Options::~Options()
{
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
	fprintf(f, "  -a, --no-audio       disable audible beep\n");
	fprintf(f, "  -f, --fullscreen     initialize in full screen\n");
	fprintf(f, "  -r, --renderer       chose a renderer\n");
	fprintf(f, "      --help           display this help and exit\n");
	fprintf(f, "      --version        display version information and exit\n");
	fprintf(f, "\n");
	fprintf(f, "COMMAND must be the last argument, and sent directly to the terminal on\n");
	fprintf(f, "initialization, along with a new line.\n");
	fprintf(f, "\n");
	fprintf(f, "Keys:\n");
	fprintf(f, "  CTRL+F11             full screen\n");
	//fprintf(f, "  CTRL+SHIFT+F11       full screen with 80 columns\n");
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


#include "render/simple.h"
#include "render/monochrome.h"

Renderer* 
Options::InitializeRenderer() const
{
	if(renderer == "simple")
		return new Simple();
	else if(renderer == "monochrome")
		return new Monochrome();
	else
		throw string("invalid renderer (supported: simple, monochrome)");
}
