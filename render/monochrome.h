#ifndef MONOCHROME_H
#define MONOCHROME_H

#include "render/renderer.h"

class Monochrome : public Renderer
{
public:
	Monochrome();
	~Monochrome();
	void Render() const;
	void Resize(int scr_w, int scr_h, int& ts_w, int& ts_h) const;
	void CharPosition(int mx, int my, int& x, int& y) const;

private:
	mutable int w, h;
	mutable SDL_Texture* texture;
};

#endif
