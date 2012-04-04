#include "terminal.h"

#include "SDL.h"
#include <iostream>
using namespace std;

#include "config.h"

Terminal::Terminal(Options const& options, string const& term)
	: w(80), h(25), cursor_x(0), cursor_y(0), 
	  current_attr(NORMAL),
	  scroll_top(0), scroll_bottom(h-1), console(new Console(term)), 
	  options(options), ch(new TerminalChar*[w]), 
	  old_cursor_x(0), old_cursor_y(0), blink_on(true), 
	  last_blink(SDL_GetTicks()), escape_mode(false)
#ifndef DEBUG
	  , debug(options.debug_terminal), dx(0)
#endif
{
	SDL_EnableUNICODE(1);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, 
			SDL_DEFAULT_REPEAT_INTERVAL);

	for(int x(0); x<w; x++)
		ch[x] = new TerminalChar[h];

	UpdateCursorPosition();
}


Terminal::~Terminal()
{
	for(int x(0); x<w; x++)
		delete[] ch[x];
	delete[] ch;
	dirty.clear();
	delete console;
}


bool
Terminal::Process()
{
	if(SDL_GetTicks() >= last_blink + options.blink_speed)
		Blink();
	if(!ConsoleInput())
	       return false;
	return ConsoleOutput();
}


bool
Terminal::ConsoleInput()
{
	// read chars and send them to the console
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
		case SDL_KEYDOWN:
#ifdef DEBUG
			Debug(e.key.keysym.unicode, false);
#endif
			if(!SpecialKeyPress(e.key.keysym))
			{
				uint16_t c = e.key.keysym.unicode;
				if(c != 0)
					KeyPress(c);
			}
			break;
		case SDL_QUIT:
			return false;
		}
	}
	return true;
}


bool
Terminal::ConsoleOutput()
{
	// read console and draw on terminal
	string s;
	int ret;
	if((ret = console->ReceiveString(s)) == Console::READ_OK)
	{
		string::const_iterator it;
		for(it = s.begin(); it != s.end(); it++)
		{
#ifdef DEBUG
			Debug(*it, true);
#endif
			if(escape_mode)
				AddEscapeChar(*it);
			else
				PrintChar(*it);
		}
		return true;
	}
	else if(ret == EOF)
		return false;

	return true;
}


inline void
Terminal::KeyPress(uint16_t key)
{
	console->SendChar((uint8_t)key);
}


void 
Terminal::PrintString(string const& s)
{
	string::const_iterator it;
	for(it = s.begin(); it < s.end(); it++)
		PrintChar(*it);
}


void 
Terminal::PrintChar(const uint8_t c)
{
	switch(c)
	{
	case 15:
		// TODO - alternate charset???
		break;
	case 27: // ESC
		escape_mode = true;
		escape_sequence = string(1, (char)27);
		break;
	case '\a':
		cout << "Beep!" << endl; // TODO
		break;
	case '\t': // tab
		AdvanceCursorX();
		while((cursor_x % 8) != 0)
			AdvanceCursorX();
		break;
	case '\n': // new line
		AdvanceCursorY();
		break;
	case '\r': // carriage return
		cursor_x = 0;
		UpdateCursorPosition();
		break;
	case '\b':
		if(cursor_x > 0)
		{
			//SetChar(cursor_x-1, cursor_y, ' ', NORMAL);
			--cursor_x;
			UpdateCursorPosition();
		}
		break;
	default:
		SetChar(cursor_x, cursor_y, c, current_attr);
		AdvanceCursorX();
	}

	blink_on = false;
	Blink();
}


void 
Terminal::SetChar(const int x, const int y, uint8_t c, CharAttr attr)
{
	if(x < 0 || y < 0 || x >= w || y >= h)
		abort();
	ch[x][y].ch = c;
	ch[x][y].attr = attr;
	dirty.insert(y*w+x);
}


void 
Terminal::AddEscapeChar(const uint8_t c)
{
	char ch = (char)c;

	// if it's a ESC, restart the escape sequence
	if(ch == 27)
	{
		escape_sequence = string(1, ch);
		return;
	}

	escape_sequence += (char)c;

	bool end_seq = false;
	if((ch >= '<' && ch <= 'Z' && ch != '?') || (ch >= 'a' && ch <= 'z'))
		end_seq = true;
	if(escape_sequence.length() == 3)
	{
		char in = escape_sequence[1];
		if(in == '(' || in == ')' || in == '#')
			end_seq = true;
	}

	if(end_seq)
	{
		escape_mode = false;
		ExecuteEscapeSequence(escape_sequence);
	}
}


void 
Terminal::AdvanceCursorX()
{
	cursor_x++;
	if(cursor_x >= w)
	{
		AdvanceCursorY(false);
		cursor_x = 0;
	}
	UpdateCursorPosition();
}


void 
Terminal::AdvanceCursorY(bool update)
{
	++cursor_y;

	// advance page
	if(cursor_y > scroll_bottom)
	{
		ScrollUp();
		--cursor_y;
	}

	if(update)
		UpdateCursorPosition();
}


void
Terminal::UpdateCursorPosition()
{
	if(cursor_x >= w || cursor_y >= h)
	{
		fprintf(stderr, "warning: trying to set cursor to %d,%d.\n",
				cursor_x, cursor_y);
		return;
	}

	TerminalChar* old(&ch[old_cursor_x][old_cursor_y]);
	old->cursor = false;
	SetChar(old_cursor_x, old_cursor_y, old->ch, old->attr);
	
	TerminalChar* _new(&ch[cursor_x][cursor_y]);
	_new->cursor = true;
	SetChar(cursor_x, cursor_y, _new->ch, _new->attr);

	old_cursor_x = cursor_x;
	old_cursor_y = cursor_y;
}


void
Terminal::Blink()
{
	blink_on = !blink_on;

	for(int x=0; x<w; x++)
		for(int y=0; y<h; y++)
			if(ch[x][y].attr.Reverse)
				SetChar(x, y, ch[x][y].ch, ch[x][y].attr);
	SetChar(cursor_x, cursor_y, ch[cursor_x][cursor_y].ch,
			ch[cursor_x][cursor_y].attr);

	last_blink = SDL_GetTicks();
}


void 
Terminal::InvalidEscapeSequence(string const& s)
{
	cout << "Invalid escape sequence: ";
	string::const_iterator it;
	for(it = s.begin(); it < s.end(); it++)
		if(*it == 27)
			cout << "ESC";
		else
			cout << *it;
	cout << endl;
}

void
Terminal::ExecuteEscapeSequence(string const& s)
{
	InvalidEscapeSequence(s);
}


void
Terminal::ScrollUp()
{
	for(int y(scroll_top+1); y<=scroll_bottom; y++)
		for(int x(0); x<w; x++)
			SetChar(x, y-1, ch[x][y].ch, ch[x][y].attr);
	for(int x(0); x<w; x++)
		SetChar(x, scroll_bottom, ' ', NORMAL);
}


void
Terminal::ScrollDown()
{
	for(int y(scroll_bottom-1); y>=scroll_top; y--)
		for(int x(0); x<w; x++)
			SetChar(x, y+1, ch[x][y].ch, ch[x][y].attr);
	for(int x(0); x<w; x++)
		SetChar(x, scroll_top, ' ', NORMAL);
}


bool
Terminal::SpecialKeyPress(SDL_keysym key)
{
	return false;
}


void
Terminal::SetScrollRegion(int top, int bottom)
{
	this->scroll_top = top-1;
	this->scroll_bottom = bottom-1;
//	cursor_x = 0;
//	cursor_y = top;
//	UpdateCursorPosition();
}


#ifdef DEBUG
inline void 
Terminal::Debug(const uint16_t c, const bool is_output) const
{
	if(!debug)
		return;

	char c1, c2;
	if(is_output) { c1 = '['; c2 = ']'; } else { c1 = '<'; c2 = '>'; }
	
	if(c < 32)
		printf("%c%02x%c ", c1, c, c2);
	else
		printf("%c %c%c ", c1, c, c2);
	if(dx == 14)
	{
		printf("\n");
		dx = 0;
	}

	fflush(stdout);

	dx++;
}
#endif
