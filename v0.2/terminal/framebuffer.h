#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <vector>
#include <istream>
using namespace std;

class Options;

typedef struct {
	bool Reverse    : 1;
	bool Underline  : 1;
	bool Highlight  : 1;
	bool Blink      : 1;
} Attribute;

typedef struct Char {
	char Ch;
	Attribute Attr;
	Char() : Ch(' ') { }
} Char;

class Framebuffer
{
public:
	explicit Framebuffer(Options const& options);
	~Framebuffer();

	friend istream& operator>>(istream& in, Framebuffer& fb);
	//void operator<<(const char c);

	inline int W() const { return w; }
	inline int H() const { return h; }
	inline Char Ch(int x, int y) const { return chars[x+(y*W())]; }

	mutable vector<int>* dirty;

private:
	void AdvanceCursorX();
	void AdvanceCursorY();
	void ScrollUp();

	Options const& options;
	int w, h;
	int cursor_x, cursor_y;
	int scroll_top, scroll_bottom;
	Attribute current_attr;
	vector<Char> chars;
};

#endif