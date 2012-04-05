#ifndef GRAPHICLIBRARY_H
#define GRAPHICLIBRARY_H

class Options;
class Framebuffer;

class GraphicLibrary {
public:
	typedef enum { SDL, CURSES } GraphicLibraries;

	static GraphicLibrary* Initialize(GraphicLibraries const lib, 
			int const w, int const h);
	virtual ~GraphicLibrary() { }

	virtual void Initialize(Options const& options, 
			Framebuffer const& framebuffer);
	virtual void Update() = 0;
	virtual int Input() const = 0;

protected:
	GraphicLibrary(int const w, int const h) : w(w), h(h) { }

	int w, h;
	Options const* options;
	Framebuffer const* fb;
};

#endif
