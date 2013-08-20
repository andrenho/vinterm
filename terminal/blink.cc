#include "terminal/blink.h"

#include "terminal/charmatrix.h"

Blink::Blink(const uint32_t milliseconds)
	: state(true), milliseconds(milliseconds)
{
	ResetClock();
}


void 
Blink::ResetClock()
{
	// TODO
	// last_blink = SDL_GetTicks();
	last_blink = 0;
	state = true;
}


bool 
Blink::TimeToBlink() const
{
	// TODO
	//uint32_t now = SDL_GetTicks();
	uint32_t now = 0;

	return (now - last_blink) > milliseconds;
}


void
Blink::DoBlink(CharMatrix const& cm)
{
	state = !state;

	// TODO
	// last_blink = SDL_GetTicks();
	last_blink = 0;
}
