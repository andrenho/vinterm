#ifndef SIMPLE_H
#define SIMPLE_H

#include "render/renderer.h"

class Simple : public Renderer
{
public:
	Simple(Font const& font, Framebuffer& fb);
	~Simple();
	void Render() const;
	void Resize(int scr_w, int scr_h, int& ts_w, int& ts_h) const;

private:
	mutable int w, h;
	mutable SDL_Texture* texture;
};

#endif
