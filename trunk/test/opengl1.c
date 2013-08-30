// gcc -o opengl1 opengl1.c `sdl2-config --cflags --libs` -lGL && ./opengl1

// http://duriansoftware.com/joe/An-intro-to-modern-OpenGL.-Table-of-Contents.html

#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

SDL_Renderer* ren = NULL;
SDL_Window*   win = NULL;

static struct {
	GLuint vertex_buffer, element_buffer;
	GLuint textures[2];
} g_resources;

static const GLfloat g_vertex_buffer_data[] = {
	-1.0f, -1.0f,
	 1.0f, -1.0f,
	-1.0f,  1.0f,
	 1.0f,  1.0f
};
static const GLushort g_element_buffer_data[] = { 0, 1, 2, 3 };


static GLuint make_buffer(GLenum target, const void* buffer_data, GLsizei buffer_size)
{
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(target, buffer);
	glBufferData(target, buffer_size, buffer_data, GL_STATIC_DRAW);
	return buffer;
}


static int make_resources()
{
	g_resources.vertex_buffer = make_buffer(GL_ARRAY_BUFFER,
			g_vertex_buffer_data, sizeof(g_vertex_buffer_data));
	g_resources.element_buffer = make_buffer(GL_ELEMENT_ARRAY_BUFFER,
			g_element_buffer_data, sizeof(g_element_buffer_data));
	return 1;
}


static GLuint make_texture(const char* filename)
{
	GLuint texture;
	SDL_Surface* sf = SDL_LoadBMP(filename);
	void* pixels = sf->pixels;
	if(!pixels)
		return 0;
	int width = sf->w, height = sf->h;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
}


static void update_fade_factor()
{
}


static void render()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//SDL_GL_SwapWindow(win);
	SDL_RenderPresent(ren);
}


int main()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(400, 300, 0, &win, &ren);

	if(!make_resources()) {
		fprintf(stderr, "Failed to load resources.\n");
		return 1;
	}

	int active = 1;
	SDL_Event e;
	while(active) {
		while(SDL_PollEvent(&e)) {
			switch(e.type) {
			case SDL_QUIT:
				active = 0;
				break;
			}
		}
		render();
	}
}
