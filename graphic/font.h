#ifndef FONT_H
#define FONT_H

#include <string>
using namespace std;

class Options;
class Framebuffer;

class Font
{
public:
	virtual ~Font() { }

	static Font* LoadFont(Options const& options);
	virtual string Encoding() const = 0;
	virtual void DrawChar(uint32_t ch, Framebuffer& fb, int x, int y) const = 0;
	virtual int CharWidth() const = 0;
	virtual int CharHeight() const = 0;

protected:
	Font() { }
};

#endif
