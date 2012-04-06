#include "terminal/pty.h"

#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <pty.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>


PTY::PTY(const string terminal)
{
	CopyStartupFile();       
	OpenPTY(terminal);
}


PTY::~PTY()
{
	close(fd);
}


void 
PTY::CopyStartupFile()
{
	system("cp -n " DATADIR "/vinterm_profile $HOME/.vinterm_profile");
}


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
		setenv("LINES", "24", 1);
		setenv("COLUMNS", "80", 1);
		setenv("ENV", "$HOME/.vinterm_profile", 1);
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
		return c;
}


void 
PTY::Send(const char c)
{
	if(write(fd, &c, 1) == -1)
	{
		perror("write");
		abort();
	}
}
