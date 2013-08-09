#ifndef MOUSE_H
#define MOUSE_H

#include <cstdint>
#include <deque>
#include <string>
using namespace std;

class Terminal;

class Mouse
{
public:
	Mouse();

	void SetMode(int n);
	void ResetMode(int n);

	void AddButtonPressToQueue(deque<uint32_t>& keyQueue, bool press, 
			int x, int y, int button, 
			bool shift, bool meta, bool ctrl, bool motion=false);
	void Drag(deque<uint32_t>& keyQueue, int x, int y, uint8_t button);

	string Translate(int ch, deque<uint32_t>& keyQueue);
	bool Captured() const;

	void setTerminal(Terminal* terminal) { this->terminal = terminal; }

private:
	int mode;
	int last_press_x, last_press_y;
	Terminal* terminal;
	bool selection_started;
};

#endif
