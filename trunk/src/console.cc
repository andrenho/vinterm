#include "console.h"

#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <pty.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

Console::Console(string terminal)
{
	CopyStartupFile();

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
		setenv("LINES", "25", 1);
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


Console::~Console()
{
	close(fd);
}


void
Console::SendChar(const char c) const
{
	if(write(fd, &c, 1) == -1)
	{
		perror("write");
		abort();
	}
}


void 
Console::SendString(const string data) const
{
	if(write(fd, data.c_str(), data.length()) == -1)
	{
		perror("write");
		abort();
	}
}


int Console::ReceiveChar() const
{
	char c;
	int nread(read(fd, &c, 1));
	if(nread == -1)
	{
		if(errno == EAGAIN)
			return NO_DATA;
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


int Console::ReceiveString(string& s) const
{
	char buf[256];
	int nread;
	
	while((nread = read(fd, buf, 255)) > 0)
	{
		buf[nread] = '\0';
		s.append(buf);
	}

	if(nread == -1)
	{
		if(errno == EAGAIN)
		{
			if(s.length() == 0)
				return NO_DATA;
			else
				return READ_OK;
		}
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
		abort();
}


void 
Console::CopyStartupFile()
{
	system("cp -n " DATADIR "/vinterm_profile $HOME/.vinterm_profile");
}
