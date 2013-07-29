#include "terminal/pty.h"

#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#if (defined __APPLE__ && defined __MACH__)
#  include <util.h>
#else
#  include <pty.h>
#endif

#include "options.h"

PTY::PTY(Options const& options, const string terminal)
	: fd(0), options(options), debug(options.debug_terminal), debug_ct(0)
{
	//CopyStartupFile();       
	OpenPTY(terminal);
}


PTY::~PTY()
{
	close(fd);
}


/*
void 
PTY::CopyStartupFile() const
{
	system("cp -n " DATADIR "/vinterm_profile $HOME/.vinterm_profile");
}
*/


void 
PTY::OpenPTY(string const& terminal)
{
	char* shell = getenv("SHELL");
	if(!shell)
		shell = (char*)"/bin/sh";
	else
		shell = &shell[5];

	pid_t pid;
	if((pid = forkpty(&fd, NULL, NULL, NULL)) < 0)
		abort();
	else if(!pid)
	{
		// child
		if (setenv("TERM", terminal.c_str(), 1))
		{
			perror("putenv");
			abort();
		}
		setenv("ENV", DATADIR "/vinterm_profile", 1);
		if(execlp(shell, "sh", (void*)0) == -1)
		{
			perror("execlp");
			abort();
		}
	}
	else
	{
		int flags;
		if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
			flags = 0;
		if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) 
		{
			perror("fcntl");
			abort();
		}
	}
}


int 
PTY::Get() const
{
	char c;
	int nread(read(fd, &c, 1));
	if(nread == -1)
	{
		if(errno == EAGAIN)
			return NO_DATA;
		else if(errno == EIO)
			return EOF;
		else
		{
			perror("read");
			abort();
		}
	}
	else if(nread == 0)
		return EOF;
	else
	{
#ifdef DEBUG
		Debug(c, false);
#endif
		return c;
	}
}


void 
PTY::Send(const uint8_t c)
{
#ifdef DEBUG
	Debug(c, true);
#endif
	if(write(fd, &c, sizeof(c)) == -1)
	{
		perror("write");
		abort();
	}
}


void 
PTY::Send(string s)
{
#ifdef DEBUG
	for(string::const_iterator c = s.begin(); c < s.end(); c++)
		Debug(*c, true);
#endif
	if(write(fd, s.c_str(), s.length()) == -1)
	{
		perror("write");
		abort();
	}
}


void 
PTY::Debug(char c, bool sending) const
{
	if(!debug)
		return;

	char c1 = '[', c2 = ']';
	if(sending)
	{
		c1 = '<';
		c2 = '>';
	}
	if(c <= 32)
		printf("%c%3d%c ", c1, (unsigned char)c, c2);
	else
		printf("%c %c %c ", c1, c, c2);
	debug_ct++;
	if(debug_ct > 12)
	{
		printf("\n");
		debug_ct = 0;
	}
	fflush(stdout);
}


void
PTY::Resize(uint16_t w, uint16_t h)
{
	struct winsize ws = { h, w, 0, 0 };
	if(ioctl(fd, TIOCSWINSZ, &ws) < 0)
		fprintf(stderr, "Couldn't set window size.\n");
	
}
