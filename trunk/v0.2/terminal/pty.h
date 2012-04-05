#ifndef PTY_H
#define PTY_H

#include <string>
#include <ostream>
using namespace std;

class PTY
{
public:
	explicit PTY(const string terminal="vinterm");
	~PTY();

	friend ostream& operator<<(ostream& out, PTY const& pty);

private:
	void CopyStartupFile();
	void OpenPTY(string const& terminal);

	int fd;
	mutable bool active;
};

#endif
