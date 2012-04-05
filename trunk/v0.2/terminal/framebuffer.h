#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <set>
#include <vector>
#include <istream>
using namespace std;

#include "terminal/charattr.h"
class Options;

class Framebuffer
{
public:
	explicit Framebuffer(Options const& options);
	~Framebuffer();

	void Put(const char c);
	void AdvanceCursorX();
	void AdvanceCursorY();
	void CarriageReturn();

	inline int W() const { return w; }
	inline int H() const { return h; }
	inline Char Ch(int x, int y) const { return chars[x+(y*W())]; }

	mutable set<int>* dirty;

private:
	void ScrollUp();

	Options const& options;
	int w, h;
	int cursor_x, cursor_y;
	int scroll_top, scroll_bottom;
	Attribute current_attr;
	vector<Char> chars;
};

#endif
