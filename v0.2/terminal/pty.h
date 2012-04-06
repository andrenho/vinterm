#ifndef PTY_H
#define PTY_H

#include <string>
#include <ostream>
using namespace std;

class PTY
{
public:
	enum { NO_DATA = -100 };

	explicit PTY(const string terminal="vinterm");
	~PTY();

	int Get() const;
	void Send(const char c);

private:
	void CopyStartupFile();
	void OpenPTY(string const& terminal);

	int fd;
	mutable bool active;
};

#endif
