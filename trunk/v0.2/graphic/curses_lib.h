#ifndef CURSES_LIB
#define CURSES_LIB

#ifdef _CURSES

#include "graphic/graphiclibrary.h"

class Curses : public GraphicLibrary
{
public:
	Curses(int const w, int const h);
	~Curses();

	void Update();
	int Input() const;
};

#endif
#endif
