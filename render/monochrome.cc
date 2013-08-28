#include "render/monochrome.h"

#include <iostream>
using namespace std;

#include "graphic/font.h"
#include "graphic/framebuffer.h"
#include "util/chronometer.h"

Monochrome::Monochrome()
	: Renderer(), texture(nullptr)
{
}


Monochrome::~Monochrome()
{
}


void 
Monochrome::Resize(int scr_w, int scr_h, int& ts_w, int& ts_h) const
{
}


void
Monochrome::Render() const
{
}


void
Monochrome::CharPosition(int mx, int my, int& x, int& y) const
{
}
