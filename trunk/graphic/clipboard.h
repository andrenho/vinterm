#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <string>
using namespace std;

class Clipboard
{
public:
	Clipboard();
	~Clipboard();

	string Read();
};

#endif
