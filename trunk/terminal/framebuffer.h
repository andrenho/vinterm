#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <set>
#include <vector>
#include <istream>
using namespace std;

#include "terminal/backtrack.h"
#include "terminal/charattr.h"
class Options;
class Blink;

typedef enum { UP, DOWN, LEFT, RIGHT } Direction;
typedef enum { 
	NONE, HIGHLIGHT, UNDERLINE, BLINK, REVERSE, DIM, INVISIBLE
} AttrType;
typedef enum { VERY_VISIBLE=2, VISIBLE=1, NOT_VISIBLE=0 } CrsVisibility;


struct Selection
{
	Selection() { Reset(); }
	void Reset() { 
		start_backtrack = -1; start_x = -1; start_y = -1;
		end_backtrack = -1; end_x = -1; end_y = -1;
	}
	bool Active() const { return start_backtrack != -1 && end_backtrack != -1; }

	int start_backtrack, start_x, start_y;
	int end_backtrack, end_x, end_y;
};


class Framebuffer
{
public:
	Framebuffer(Options const& options);
	~Framebuffer();

	// backtracking
	void BackTrack();
	void ForeTrack();
	void ForetrackToScreen();

	// resize screen
	void Resize(int w, int h);

	// write on the screen
	void Put(const char c, bool ignore_insert_mode=true);
	void Put(const char c, const int x, const int y, 
			bool ignore_insert_mode=true);
	void Put(const char c, Attribute attr, const int x, const int y,
			bool ignore_insert_mode=true);

	// methods that move the cursor
	bool AdvanceCursorY();
	void RecedeCursorY();
	void CarriageReturn();
	void Tab();
	void Backspace();
	void MoveCursor(Direction dir, int moves);
	void SetCursorPosition(int x, int y);

	// scrolling methods
	void ScrollUp();
	void ScrollDown();
	void SetScrollingRegion(int top, int bottom);

	// add text
	void AddLinesBelowCursor(int n);

	// clear text
	void ClearRow(bool upto_cursor, int y=-1);
	void DeleteLines(int n);
	void DeleteChars(int n);
	void EraseChars(int n);

	// save/restore screen
	void SaveScreen();
	void RestoreScreen();

	// insert
	void InsertChars(int n);

	// bells
	void Flash(bool reverse=true);
	bool Flashing() const { return flashing; }

	// information about the framebuffer
	Char Ch(int x, int y) const;
	inline int W() const { return w; }
	inline int H() const { return h; }
	inline int CursorX() const { return cursor_x; }
	inline int CursorY() const { return cursor_y; }

	// attributes
	void RegisterBlinks() const;
	void SetAttr(AttrType attr, bool value);

	// selection
	void SetStartSelection(int x, int y);
	void SetEndSelection(int x, int y);
	void SetNoSelection();

	mutable set<int>* dirty;
	int InsertMode;
	CrsVisibility CursorVisibility;
	Blink* blink;

private:
	void ValidateCursorPosition();
	bool IsSelected(int x, int y) const;

	Attribute current_attr;
	int w, h;
	int cursor_x, cursor_y;
	int scroll_top, scroll_bottom;
	vector<Char> chars, saved_screen;
	int saved_x, saved_y;
	set<int> tabs;
	bool flashing;
	Backtrack* backtrack;
	int current_backtrack;
	Selection selection;
};

#endif
