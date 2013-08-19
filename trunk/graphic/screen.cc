#include "graphic/screen.h"

Screen::Screen(Options const& options, Framebuffer const& fb, Mouse& mouse)
	: options(options), fb(fb), mouse(mouse)
{
}


void
Screen::Resize(int new_w, int new_h, int full_screen, int& ts_w, int& ts_h)
{
}


void
Screen::Update()
{
}


Screen::~Screen()
{
}
