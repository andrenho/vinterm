#ifndef KEYQUEUE_H
#define KEYQUEUE_H

#include <cstdint>

#include <deque>
using namespace std;

enum { 
	F1=256, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	K_UP, K_DOWN, K_LEFT, K_RIGHT, HOME, DELETE, PAGE_UP, PAGE_DOWN,
	INSERT, END, SH_UP, SH_DOWN, SH_LEFT, SH_RIGHT, CT_PAGE_UP,
	CT_PAGE_DOWN, SH_PAGE_UP, SH_PAGE_DOWN, QUIT, RESIZE,
	MPRESS, MRELEASE, MDRAG
};

extern deque<uint32_t> keyQueue;

#endif
