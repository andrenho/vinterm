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
			int button, bool shift, bool meta, bool ctrl) const
{
	if(mode < 1000)
		return;

	if(press)
		keyQueue.push_back(MPRESS);
	else
		return;
	keyQueue.push_back(button);
	keyQueue.push_back((shift << 2) | (meta << 1) | ctrl);
	keyQueue.push_back(x);
	keyQueue.push_back(y);
}


string 
Mouse::Translate(int ch, deque<uint32_t>& keyQueue)
{
	string s;
	s += (char)27;
	s += "[";
	s += 'M';

	// button
	s += (char)32;
	keyQueue.pop_front();
	keyQueue.pop_front();

	// x
	int x = keyQueue[0] + 33;
	keyQueue.pop_front();
	s += (char)x;

	// y
	int y = keyQueue[0] + 33;
	keyQueue.pop_front();
	s += (char)y;

	return s;
}
