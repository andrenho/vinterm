#include "render/renderer.h"

#include "graphic/screen.h"

Renderer::Renderer()
{
	ren = screen->GLRenderer();
}
