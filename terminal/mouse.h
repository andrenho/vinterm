#ifndef MOUSE_H
#define MOUSE_H

#include <cstdint>
#include <ctime>
#include <deque>
#include <string>
using namespace std;

#include "global.h"

class Mouse
{
public:
	Mouse();

	void SetMode(int n);
	void ResetMode(int n);

	void AddButtonPressToQueue(bool press, int x, int y, int button, 
			bool shift, bool meta, bool ctrl, bool motion=false);
	void Drag(int x, int y, uint8_t button);

	string Translate(int ch);
	bool Captured() const;

private:
	int mode;
	int last_press_x, last_press_y;
	Terminal* terminal;
	bool selection_started;
};

#endif
