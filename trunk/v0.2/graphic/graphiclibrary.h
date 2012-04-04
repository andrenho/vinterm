#ifndef GRAPHICLIBRARY_H
#define GRAPHICLIBRARY_H

class Options;
class Framebuffer;

class GraphicLibrary {
public:
	typedef enum { SDL } GraphicLibraries;

	static GraphicLibrary* Initialize(GraphicLibraries const lib, 
			int const w, int const h);
	virtual ~GraphicLibrary() { }

	virtual void Initialize(Options const& options, 
			Framebuffer const& framebuffer);
	virtual void Update() = 0;

protected:
	Options const* options;
	Framebuffer const*  framebuffer;
};

#endif
