#ifndef PTY_H
#define PTY_H

#include <string>
#include <ostream>
using namespace std;

class PTY
{
public:
	enum { NO_DATA = -100 };

	explicit PTY(bool debug_terminal=false, 
			const string terminal="vinterm");
	~PTY();

	int Get() const;
	void Send(const char c);

private:
	void CopyStartupFile() const;
	void OpenPTY(string const& terminal);
	void Debug(char c, bool sending) const;

	int fd;
	mutable bool active;
	const bool debug;
	mutable int debug_ct;
};

#endif
