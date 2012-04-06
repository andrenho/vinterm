#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <set>
#include <vector>
#include <istream>
using namespace std;

#include "terminal/charattr.h"
class Options;

typedef enum { UP, DOWN, LEFT, RIGHT } Direction;

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	void Put(const char c);
	void Put(const char c, const int x, const int y);
	void Put(const char c, Attribute attr, const int x, const int y);

	// methods that move the cursor
	void AdvanceCursorX();
	void AdvanceCursorY();
	void CarriageReturn();
	void Tab();
	void Backspace();
	void MoveCursor(Direction dir, int moves);
	void Home();
	void LowerLeft();

	inline int W() const { return w; }
	inline int H() const { return h; }
	inline int CursorX() const { return cursor_x; }
	inline int CursorY() const { return cursor_y; }
	inline Char Ch(int x, int y) const { return chars[x+(y*W())]; }

	mutable set<int>* dirty;

private:
	void ScrollUp();
	void ScrollDown();

	int w, h;
	int cursor_x, cursor_y;
	int scroll_top, scroll_bottom;
	Attribute current_attr;
	vector<Char> chars;
};

#endif
