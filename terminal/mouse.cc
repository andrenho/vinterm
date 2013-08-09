#include "terminal/mouse.h"

#include <iostream>

#include "graphic/screen.h"

void 
Mouse::SetMode(int n)
{
	mode = n;
	cout << "Mode set to " << n << endl;
}


void 
Mouse::ResetMode(int n)
{
	mode = 0;
}


void 
Mouse::AddButtonPressToQueue(deque<uint32_t>& keyQueue, bool press, int x, int y, 
			int button, bool shift, bool meta, bool ctrl, bool motion)
{
	last_press_x = x;
	last_press_y = y;

	// check mode
	if(mode < 1000)
		return;

	// add to queue
	if(motion)
		keyQueue.push_back(MDRAG);
	else if(press)
		keyQueue.push_back(MPRESS);
	else
		keyQueue.push_back(MRELEASE);
	keyQueue.push_back(button);
	keyQueue.push_back((shift << 2) | (meta << 1) | ctrl);
	keyQueue.push_back(x);
	keyQueue.push_back(y);
}


void 
Mouse::Drag(deque<uint32_t>& keyQueue, int x, int y, uint8_t button)
{
	if(x != last_press_x || y != last_press_y)
		AddButtonPressToQueue(keyQueue, true, x, y, button, 
				false, false, false, true);
}


string 
Mouse::Translate(int ch, deque<uint32_t>& keyQueue)
{
	string s;
	s += (char)27;
	s += "[";
	s += 'M';

	// button
	int button = keyQueue[0];
	keyQueue.pop_front();
	int keys = keyQueue[0];
	keyQueue.pop_front();

	uint8_t b = 32;
	if(ch == MDRAG)
		b += 32;
	else if(ch == MRELEASE)
		b += 3;
	
	if(ch == MDRAG || ch == MPRESS)
	{
		if(button <= 3)
			b += (button-1);
		else if(button == 4)
			b += 64;
		else if(button == 5)
			b += 65;
	}
	if(keys & 0x4) // shift
		b += 4;
	if(keys & 0x2) // meta
		b += 8;
	if(keys & 0x1) // control
		b += 16;
	s += (char)b;

	// x
	uint8_t x = keyQueue[0] + 33;
	keyQueue.pop_front();
	s += (char)x;

	// y
	uint8_t y = keyQueue[0] + 33;
	keyQueue.pop_front();
	s += (char)y;

	return s;
}


bool 
Mouse::Captured() const
{
	return mode > 1000;
}
