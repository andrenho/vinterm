#include "vt100.h"

#include <cstdlib>
#include <iostream>
using namespace std;

void 
VT100::ExecuteEscapeSequence(string const& seq)
{
	if(seq[1] == '[')
	{
		char command;
		vector<int> parameters;
		if(!ParseCommand(seq, command, parameters))
			InvalidEscapeSequence(seq);
		switch(command)
		{
		case 'A':
			if(parameters.size() == 0)
				MoveCursorUp(1);
			else
				MoveCursorUp(parameters[0]);
			break;
		case 'B':
			if(parameters.size() == 0)
				MoveCursorDown(1);
			else
				MoveCursorDown(parameters[0]);
			break;
		case 'C':
			if(parameters.size() == 0)
				MoveCursorRight(1);
			else
				MoveCursorRight(parameters[0]);
			break;
		case 'D':
			if(parameters.size() == 0)
				MoveCursorLeft(1);
			else
				MoveCursorLeft(parameters[0]);
			break;
		case 'H':
			if(parameters.empty())
				ChangeCursorPosition(0, 0);
			else if(parameters.size() == 2)
				ChangeCursorPosition(parameters[1]-1, parameters[0]-1);
			else
				InvalidEscapeSequence(seq);
			break;
		case 'J':
			if(parameters.empty() || parameters[0] == 0)
				EraseFromCursorToEndOfScreen();
			else if(parameters[0] == 2)
				EraseScreen();
			else
				InvalidEscapeSequence(seq);
			break;
		case 'K':
			EraseToEOL();
			break;
		case 'c':
			if(parameters.empty() || parameters[0] == 0)
				console->SendString("\033[?1;0c");
			else
				InvalidEscapeSequence(seq);
			break;
		case 'h':
			if(parameters.size() != 1)
				InvalidEscapeSequence(seq);
			else if(parameters[0] == 1)
				cursor_keys = APPLICATION;
			else
				InvalidEscapeSequence(seq);
			break;
		case 'l':
			if(parameters.size() != 1)
				InvalidEscapeSequence(seq);
			else if(parameters[0] == 1)
				cursor_keys = CURSOR;
			else
				InvalidEscapeSequence(seq);
			break;
		case 'm':
			if(parameters.empty())
				current_attr = NORMAL;
			for(vector<int>::iterator it = parameters.begin();
					it < parameters.end(); ++it)
			{
				if(*it == 0)
					current_attr = NORMAL;
				else if(*it == 1)
					current_attr.Highlight = true;
				else if(*it == 4)
					current_attr.Underline = true;
				else if(*it == 5)
					current_attr.Blink = true;
				else if(*it == 7)
					current_attr.Reverse = true;
				else
				{
					InvalidEscapeSequence(seq);
					break;
				}
			}
			break;
		case 'r':
			if(parameters.size() != 2)
				SetScrollRegion(1, 25);
			else
				SetScrollRegion(parameters[0], parameters[1]);
			break;
		default:
			Terminal::ExecuteEscapeSequence(seq);
		}
	}
	else 
		switch(seq[1])
		{
		case '=':
			keypad_keys = APPLICATION;
			break;
		case '>':
			keypad_keys = CURSOR;
			break;
		case 'M':
			if(cursor_y > scroll_top)
				MoveCursorUp(1);
			else
				ScrollDown();
			break;
		default:
			InvalidEscapeSequence(seq);
		}
}


bool
VT100::SpecialKeyPress(SDL_keysym key)
{
	int i=0;
	while(!keycodes[i].code.empty())
	{
		if(key.sym == keycodes[i].key)
		{
			Uint8 *k = SDL_GetKeyState(NULL);

			// check for keypad
			if(k[SDLK_NUMLOCK] && key.sym >= SDLK_KP0 && key.sym <= SDLK_KP9)
			{
				KeypadPress(key.sym);
				return true;
			}

			// check for cursor or application code
			string code = keycodes[i].code;
			if(cursor_keys == APPLICATION && (key.sym == SDLK_UP || key.sym == SDLK_DOWN || key.sym == SDLK_LEFT || key.sym == SDLK_RIGHT))
				code = keycodes[i].cursor_or_application_code;
			else if(keypad_keys == APPLICATION && !keycodes[i].cursor_or_application_code.empty())
				code = keycodes[i].cursor_or_application_code;

			// add modifier
			AddModifier(k, code);

			// send code
			for(string::iterator it = code.begin();
					it < code.end();
					++it)
				KeyPress(*it);
			return true;
		}
		++i;
	}
	return false;
}


void 
VT100::AddModifier(Uint8* k, string& code)
{
	static struct {
		bool shift, control, alt;
		char num;
	} codes[] = {
		{ false, false, false, '0' },
		{ true , false, false, '2' },
		{ false, true , false, '5' },
		{ false, false, true , '3' },
		{ true , true , false, '6' },
		{ false, true , true , '7' },
		{ true , false, true , '4' },
		{ true , true , true , '8' }
	};
	
	bool shift = k[SDLK_RSHIFT] | k[SDLK_LSHIFT];
	bool control = k[SDLK_RCTRL] | k[SDLK_LCTRL];
	bool alt = k[SDLK_RALT] | k[SDLK_LALT];
	int i = 0;
	while(1)
	{
		if(codes[i].shift == shift
		&& codes[i].control == control
		&& codes[i].alt == alt)
		{
			char c = codes[i].num;
			if(c == '0')
				return;
			
			char last = code[code.length()-1];
			if(last == '~')
				code.insert(2, string() + c + ';');
			else
				code.insert(2, string("1;") + c);
			return;
		}
		++i;
	}
}



void
VT100::KeypadPress(SDLKey key)
{
	switch(key)
	{
	case SDLK_KP0: KeyPress('0'); break;
	case SDLK_KP1: KeyPress('1'); break;
	case SDLK_KP2: KeyPress('2'); break;
	case SDLK_KP3: KeyPress('3'); break;
	case SDLK_KP4: KeyPress('4'); break;
	case SDLK_KP5: KeyPress('5'); break;
	case SDLK_KP6: KeyPress('6'); break;
	case SDLK_KP7: KeyPress('7'); break;
	case SDLK_KP8: KeyPress('8'); break;
	case SDLK_KP9: KeyPress('9'); break;
	default: abort();
	}
}


bool
VT100::ParseCommand(string const& seq, char& command, vector<int>& parameters)
{
	if(seq[seq.length()-1] == 'H')
	{
		// hook
	}

	string::const_iterator it;
	unsigned int i;
	for(i = 0, it = seq.begin(); it < seq.end(); i++, it++)
	{
		char c(*it);
		if(c == 27 || c == '[' || c == '?' || c == ';') // escape
			continue;
		else if(c == '(' || c == ')')
		{
			command = c;
		}
		if((c >= 64 && c <= 126) || i == seq.size()-1) // Command
		{
			command = c;
			return true;
		}
		else
		{
			string par;
			while(((*it) < 64 || (*it) > 126) && (*it) != ';')
			{
				par += (*it);
				it++;
			}

			// TODO - replace for a C++ conversion function
			int n = strtol(par.c_str(), NULL, 10);
			parameters.push_back(n);
			it--;
		}
	}

	return false;
}


void
VT100::ChangeCursorPosition(const int x, const int y)
{
	cursor_x = x;
	cursor_y = y;
	/*
	while(cursor_y >= scroll_bottom)
	{
		ScrollUp();
		--cursor_y;
	}*/
	UpdateCursorPosition();
}


void
VT100::EraseScreen()
{
	for(int x(0); x<w; x++)
		for(int y(0); y<h; y++)
			SetChar(x, y, ' ', NORMAL);
}


void
VT100::EraseFromCursorToEndOfScreen()
{
	for(int x(0); x<w; x++)
		for(int y(cursor_y); y<h; y++)
			SetChar(x, y, ' ', NORMAL);
}


void
VT100::EraseToEOL()
{
	for(int x(cursor_x); x<w; x++)
		SetChar(x, cursor_y, ' ', NORMAL);
}


void 
VT100::MoveCursorUp(const int n)
{
	ChangeCursorPosition(cursor_x, max(0, cursor_y - n));
}


void 
VT100::MoveCursorDown(const int n)
{
	ChangeCursorPosition(cursor_x, min(h-1, cursor_y + n));
}


void 
VT100::MoveCursorLeft(const int n)
{
	ChangeCursorPosition(max(0, cursor_x - n), cursor_y);
}


void 
VT100::MoveCursorRight(const int n)
{
	ChangeCursorPosition(min(w-1, cursor_x + n), cursor_y);
}


const VT100::KeyCode VT100::keycodes[] = {
	{ SDLK_UP,	"\033[A",	"\033OA" },
	{ SDLK_DOWN,	"\033[B",	"\033OB" },
	{ SDLK_RIGHT,	"\033[C",	"\033OC" },
	{ SDLK_LEFT,	"\033[D",	"\033OD" },
	{ SDLK_KP0,	"\033[2~",      "\033Op" },
	{ SDLK_KP1,	"\033[F",       "\033Oq" },
	{ SDLK_KP2,	"\033[B",       "\033Or" },
	{ SDLK_KP3,	"\033[6~"       "\033Os" },
	{ SDLK_KP4,	"\033[D",       "\033Ot" },
	{ SDLK_KP5,	"\033[E"        "\033Ou" },
	{ SDLK_KP6,	"\033[C",       "\033Ov" },
	{ SDLK_KP7,	"\033[H",       "\033Ow" },
	{ SDLK_KP8,	"\033[A",       "\033Ox" },
	{ SDLK_KP9,	"\033[5~",      "\033Oy" },
	{ SDLK_KP_MINUS, "-",           "\033Om" },
	{ SDLK_KP_MULTIPLY, "*",        "\033Ol" },
	{ SDLK_KP_PERIOD, ".",          "\033On" },
	{ SDLK_KP_PLUS, "+",            "\033OM" },
	{ SDLK_INSERT, 	"\033[2~", 	"" },
	{ SDLK_DELETE, 	"\033[3~", 	"" },
	{ SDLK_HOME, 	"\033[H", 	"\033OH" },
	{ SDLK_END, 	"\033[F", 	"\033OF" },
	{ SDLK_PAGEUP, 	"\033[5~", 	"" },
	{ SDLK_PAGEDOWN,"\033[6~", 	"" },
	{ SDLK_F1,	"\033OP",       "" },
	{ SDLK_F2,	"\033OQ",       "" },
	{ SDLK_F3,	"\033OR",       "" },
	{ SDLK_F4,	"\033OS",       "" },
	{ SDLK_F5,	"\033[15~",     "" },
	{ SDLK_F6,	"\033[17~",     "" },
	{ SDLK_F7,	"\033[18~",     "" },
	{ SDLK_F8,	"\033[19~",     "" },
	{ SDLK_F9,	"\033[20~",     "" },
	{ SDLK_F10,	"\033[21~",     "" },
	{ SDLK_F11,	"\033[22~",     "" },
	{ SDLK_F12,	"\033[23~",     "" },
	{ SDLK_q, "" }
};
