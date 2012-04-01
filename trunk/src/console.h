#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>
using namespace std;

class Console
{
public:
	Console(string terminal="con80x25");
	~Console();
	void SendChar(const char c);
	void SendString(const string data);
	int ReceiveChar() const;
	int ReceiveString(string& s) const;

	static const int NO_DATA = 0x100;
	static const int READ_OK = 0x101;
private:
	void CopyStartupFile();

	int fd;
};

#endif
