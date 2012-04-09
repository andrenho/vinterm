#ifndef INPUT_H
#define INPUT_H

class Blink;

enum { 
	QUIT=256, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	K_UP, K_DOWN, K_LEFT, K_RIGHT, HOME, DELETE, PAGE_UP, PAGE_DOWN,
	INSERT, END, SH_UP, SH_DOWN, SH_LEFT, SH_RIGHT
};

int Event(Blink* blink);

#endif
