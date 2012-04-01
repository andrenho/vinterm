#include "terminal.h"

#include "SDL.h"
#include <iostream>
using namespace std;

#include "config.h"

Terminal::Terminal(Options const& options, string const& term)
	: w(80), h(25), cursor_x(0), cursor_y(0), 
	  current_attr(NORMAL),
	  scroll_top(0), scroll_bottom(h-1), options(options),
	  console(new Console(term)), ch(new TerminalChar*[w]), 
	  old_cursor_x(0), old_cursor_y(0), blink_on(true), 
	  last_blink(SDL_GetTicks()), escape_mode(false)
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
	uint16_t c;

	// read chars and send them to the console
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
		case SDL_KEYDOWN:
			switch(e.key.keysym.sym)
			{
			case SDLK_UP: SpecialKeyPress(UP); break;
			case SDLK_DOWN: SpecialKeyPress(DOWN); break;
			case SDLK_RIGHT: SpecialKeyPress(RIGHT); break;
			case SDLK_LEFT: SpecialKeyPress(LEFT); break;
			case SDLK_KP0: SpecialKeyPress(KP0); break;
			case SDLK_KP1: SpecialKeyPress(KP1); break;
			case SDLK_KP2: SpecialKeyPress(KP2); break;
			case SDLK_KP3: SpecialKeyPress(KP3); break;
			case SDLK_KP4: SpecialKeyPress(KP4); break;
			case SDLK_KP5: SpecialKeyPress(KP5); break;
			case SDLK_KP6: SpecialKeyPress(KP6); break;
			case SDLK_KP7: SpecialKeyPress(KP7); break;
			case SDLK_KP8: SpecialKeyPress(KP8); break;
			case SDLK_KP9: SpecialKeyPress(KP9); break;
			case SDLK_KP_PERIOD: SpecialKeyPress(KP_DOT); break;
			case SDLK_KP_MULTIPLY: SpecialKeyPress(KP_STAR); break;
			case SDLK_KP_MINUS: SpecialKeyPress(KP_MINUS); break;
			case SDLK_KP_PLUS: SpecialKeyPress(KP_PLUS); break;
			default: 
				c = e.key.keysym.unicode;
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
#ifdef DEBUG
	if(options.debug_terminal)
		printf(">> %c [%d]\n", key, key);
#endif
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
#ifdef DEBUG
		if(options.debug_terminal)
			printf("<< %c [%d] (%d,%d)\n", c, c, cursor_x, cursor_y);
#endif
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
#ifdef DEBUG
		// print escape sequence
		if(options.debug_terminal)
		{
			cout << "<< ";
			for(string::const_iterator it(escape_sequence.begin()); 
					it < escape_sequence.end(); 
					it++)
			{
				if(*it == 27)
					cout << "ESC";
				else
					cout << *it;
			}
			cout << endl;
		}
#endif
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


void 
Terminal::SpecialKeyPress(SpecialKey key)
{
	switch(key)
	{	
	case UP: KeyPress(27); KeyPress('['); KeyPress('A'); break;
	case DOWN: KeyPress(27); KeyPress('['); KeyPress('B'); break;
	case RIGHT: KeyPress(27); KeyPress('['); KeyPress('C'); break;
	case LEFT: KeyPress(27); KeyPress('['); KeyPress('D'); break;
	case KP0: KeyPress('0'); break;
	case KP1: KeyPress('1'); break;
	case KP2: KeyPress('2'); break;
	case KP3: KeyPress('3'); break;
	case KP4: KeyPress('4'); break;
	case KP5: KeyPress('5'); break;
	case KP6: KeyPress('6'); break;
	case KP7: KeyPress('7'); break;
	case KP8: KeyPress('8'); break;
	case KP9: KeyPress('9'); break;
	case KP_DOT: KeyPress('.'); break;
	case KP_STAR: KeyPress('*'); break;
	case KP_PLUS: KeyPress('+'); break;
	case KP_MINUS: KeyPress('-'); break;
	}
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
