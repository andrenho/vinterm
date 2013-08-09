#ifndef MOUSE_H
#define MOUSE_H

#include <cstdint>
#include <deque>
#include <string>
using namespace std;

class Mouse
{
public:
	Mouse() : mode(0), last_press_x(-1), last_press_y(-1) { }

	void SetMode(int n);
	void ResetMode(int n);

	void AddButtonPressToQueue(deque<uint32_t>& keyQueue, bool press, 
			int x, int y, int button, 
			bool shift, bool meta, bool ctrl, bool motion=false);
	void Drag(deque<uint32_t>& keyQueue, int x, int y, uint8_t button);

	string Translate(int ch, deque<uint32_t>& keyQueue);
	bool Captured() const;

private:
	int mode;
	int last_press_x, last_press_y;
};

#endif
