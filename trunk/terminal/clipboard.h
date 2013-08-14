#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <string>
using namespace std;

class Clipboard
{
public:
	void ConnectToWM();
	string Read() const;
	void Store(string s);
	void Respond() const;

private:
	string str_clipboard;
	void* display;
	unsigned long window;
};

#endif
