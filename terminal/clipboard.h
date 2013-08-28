#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <string>
using namespace std;

#include "SDL2/SDL.h"

class Clipboard
{
public:
	void ConnectToWM(SDL_Window* w);
	string Read() const;
	void Store(string s);
	void Respond() const;

private:
	string str_clipboard;
	void* display;
	unsigned long window;
};

#endif
