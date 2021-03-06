#ifndef CHARMATRIX_H
#define CHARMATRIX_H

#include <set>
#include <vector>
#include <istream>
using namespace std;

#include "global.h"
#include "terminal/backtrack.h"
#include "terminal/blink.h"
#include "terminal/charattr.h"

typedef enum { UP, DOWN, LEFT, RIGHT } Direction;
typedef enum { 
	NONE, HIGHLIGHT, UNDERLINE, BLINK, REVERSE, DIM, INVISIBLE
} AttrType;
typedef enum { VERY_VISIBLE=2, VISIBLE=1, NOT_VISIBLE=0 } CrsVisibility;


struct Selection
{
	Selection() { Reset(); }
	void Reset() { start = -1; end = -1; } 
	bool Active() const { return start != -1 && end != -1; }

	int start;
	int end;
};


class CharMatrix
{
public:
	CharMatrix();
	~CharMatrix();

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
	void CheckForBlink();

	// selection
	void SetStartSelection(int x, int y);
	void SetEndSelection(int x, int y);
	void SetNoSelection();
	bool IsSelected(int x, int y) const;

	// terminal title
	inline string TerminalTitle() const { return terminal_title; }
	inline void setTerminalTitle(string s) { terminal_title = s; }

	mutable set<int>* dirty;
	int InsertMode;
	CrsVisibility CursorVisibility;
	Blink* blink;

private:
	void ValidateCursorPosition();

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
	int screen_advances; // screen advance (new line at bottom) counter
	string terminal_title;
	Selection selection;
	int old_cursor_x, old_cursor_y;
};

#endif
