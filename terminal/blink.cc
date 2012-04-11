#include "terminal/blink.h"

#include "SDL.h"
#include "terminal/framebuffer.h"

Blink::Blink(const int milliseconds)
	: state(true), milliseconds(milliseconds)
{
	ResetClock();
}


void 
Blink::ResetClock()
{
	last_blink = SDL_GetTicks();
	state = true;
}


bool 
Blink::TimeToBlink() const
{
	uint32_t now = SDL_GetTicks();

	return (now - last_blink) > milliseconds;
}


void
Blink::DoBlink(Framebuffer const& fb)
{
	state = !state;
	last_blink = SDL_GetTicks();
}
