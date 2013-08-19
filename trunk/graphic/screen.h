#ifndef SCREEN_H
#define SCREEN_H

#include <vector>
#include <deque>
using namespace std;

#include "graphic/audio.h"

class Options;
class Framebuffer;
class Chars;
class Font;
class Mouse;

class Screen
{
public:
	Screen(Options const& options, Framebuffer const& fb, Mouse& mouse);
	~Screen();

	void Update();
	void Resize(int new_w, int new_h, int full_screen, 
			int& ts_w, int& ts_h);

private:
	Options const& options;
	Framebuffer const& fb;
	Mouse& mouse;
};

#endif
