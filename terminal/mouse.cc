#include "terminal/mouse.h"

#include <iostream>

#include "graphic/screen.h"
#include "terminal/terminal.h"
#include "terminal/framebuffer.h"

Mouse::Mouse() 
	: mode(0), last_press_x(-1), last_press_y(-1), selection_started(false)
{ 
}

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

	// check mouse application mode
	if(mode >= 1000)
	{
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

	// mouse terminal selection mode
	if(mode < 1000 && !motion)
	{
		if(button == 1)
		{
			if(press)
				terminal->FB().SetNoSelection();
			else
				selection_started = false;
		}
		if(button == 2 && press && !motion)
			terminal->PasteFromClipboard();
	}
}


void 
Mouse::Drag(deque<uint32_t>& keyQueue, int x, int y, uint8_t button)
{
	// only if the mouse has changed character
	if(x == last_press_x && y == last_press_y)
		return;

	// mode 1002 (application mode with motion)
	if(mode >= 1002)
		AddButtonPressToQueue(keyQueue, true, x, y, button, 
				false, false, false, true);

	// mouse terminal selection mode
	if(mode < 1000)
	{
		if(!selection_started)
		{
			selection_started = true;
			terminal->FB().SetStartSelection(last_press_x, last_press_y);
		}
		terminal->FB().SetEndSelection(x, y);
	}

	last_press_x = x;
	last_press_y = y;
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
