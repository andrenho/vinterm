#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>
#include <set>
#include "SDL.h"
using namespace std;

#include "font.h"
#include "options.h"
#include "console.h"

typedef enum { NORMAL=0, REVERSE, NUM_ATTRS } CharAttr;


typedef struct TerminalChar {
	TerminalChar() : ch(' '), attr(NORMAL), cursor(false) { }
	uint8_t ch;
	CharAttr attr;
	bool cursor;
	bool isReverse() const { return attr == REVERSE; }
} TerminalChar;


class Terminal
{
public:
	explicit Terminal(Options const& options, string const& term="console");
	virtual ~Terminal();

	bool Process();

	TerminalChar Ch(int x, int y) const { return ch[x][y]; }
	bool BlinkOn() const { return blink_on; }

	const int w, h;
	mutable set<int> dirty;

protected:
	virtual void ExecuteEscapeSequence(string const& s);

private:
	bool ConsoleInput();
	bool ConsoleOutput();

	void KeyPress(uint16_t key);
	void PrintString(string const& s);
	void PrintChar(const uint8_t c);
	void SetChar(const int x, const int y, 
			const uint8_t c, const CharAttr attr);
	void AddEscapeChar(const uint8_t c);
	void AdvanceCursorX();
	void AdvanceCursorY();
	void UpdateCursorPosition();
	void Blink();

	Options const& options;
	Console* const console;
	TerminalChar** const ch;
	int cursor_x, cursor_y, old_cursor_x, old_cursor_y;
	bool blink_on;
	uint32_t last_blink;

	bool escape_mode;
	string escape_sequence;
};

#endif
