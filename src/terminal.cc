#include "terminal.h"

#include "SDL.h"
#include <iostream>
using namespace std;

Terminal::Terminal(Options const& options, Console& console)
	: w(80), h(25), options(options), console(console), 
	  ch(new TerminalChar*[w]), cursor_x(0), cursor_y(0),
	  old_cursor_x(0), old_cursor_y(0), blink_on(true), 
	  last_blink(SDL_GetTicks()), escape_mode(false)
{
	SDL_EnableUNICODE(1);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, 
			SDL_DEFAULT_REPEAT_INTERVAL);

	for(int x=0; x<w; x++)
		ch[x] = new TerminalChar[h];

	UpdateCursorPosition();

	PrintString("This is a VERY EARLY ALHPA RELEASE of Vintage Terminal. Don't expect things\r\nto work correctly at al. Use at YOUR OWN RISK.\r\n\r\n");
}


Terminal::~Terminal()
{
	for(int x=0; x<w; x++)
		delete[] ch[x];
	delete[] ch;
	dirty.clear();
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
	if((ret = console.ReceiveString(s)) == Console::READ_OK)
	{
		string::const_iterator it;
		for(it = s.begin(); it != s.end(); it++)
			if(escape_mode)
				AddEscapeChar(*it);
			else
				PrintChar(*it);
		return true;
	}
	else if(ret == EOF)
		return false;

	return true;
}


void
Terminal::KeyPress(uint16_t key)
{
	//printf("%d\n", key);
	console.SendChar((uint8_t)key);
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
	case 27: // ESC
		escape_mode = true;
		escape_sequence = string(1, (char)27);
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
			SetChar(cursor_x-1, cursor_y, ' ', NORMAL);
			--cursor_x;
			UpdateCursorPosition();
		}
		break;
	default:
		SetChar(cursor_x, cursor_y, c, NORMAL);
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
	escape_sequence += (char)c;
	if((escape_sequence.length() == 2 && (char)c != '[')
	|| (escape_sequence.length() > 2 && c >= 64 && c <= 126))
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
		AdvanceCursorY();
		cursor_x = 0;
	}
	UpdateCursorPosition();
}


void 
Terminal::AdvanceCursorY()
{
	++cursor_y;

	// advance page
	if(cursor_y >= h)
	{
		for(int y=1; y<h; y++)
			for(int x=0; x<w; x++)
				SetChar(x, y-1, ch[x][y].ch, ch[x][y].attr);
		for(int x=0; x<w; x++)
			SetChar(x, h-1, ' ', NORMAL);
		--cursor_y;
	}
	UpdateCursorPosition();
}


void
Terminal::UpdateCursorPosition()
{
	if(cursor_x >= w || cursor_y >= h)
		return;

	TerminalChar* old = &ch[old_cursor_x][old_cursor_y];
	old->cursor = false;
	SetChar(old_cursor_x, old_cursor_y, old->ch, old->attr);
	
	TerminalChar* _new = &ch[cursor_x][cursor_y];
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
			if(ch[x][y].isReverse())
				SetChar(x, y, ch[x][y].ch, ch[x][y].attr);
	SetChar(cursor_x, cursor_y, ch[cursor_x][cursor_y].ch,
			ch[cursor_x][cursor_y].attr);

	last_blink = SDL_GetTicks();
}


void 
Terminal::ExecuteEscapeSequence(string const& s)
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
