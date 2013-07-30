#ifndef MOUSE_H
#define MOUSE_H

#include <cstdint>
#include <deque>
#include <string>
using namespace std;

class Mouse
{
public:
	Mouse() : mode(0) { }

	void SetMode(int n);
	void ResetMode(int n);

	void AddButtonPressToQueue(deque<uint32_t>& keyQueue, bool press, 
			int x, int y, int button, 
			bool shift, bool meta, bool ctrl) const;
	string Translate(int ch, deque<uint32_t>& keyQueue);

private:
	int mode;
};

#endif
