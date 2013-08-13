#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <string>
using namespace std;

class Clipboard
{
public:
	Clipboard();
	~Clipboard();
	
	void GetWMInfo();
	string Read();
	void Store(string s);
	void Respond();

private:
	string str_clipboard;
	void* display;
	unsigned long window;
};

#endif
