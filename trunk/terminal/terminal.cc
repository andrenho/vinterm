#include "terminal/terminal.h"

#include <iostream>
using namespace std;

#include "options.h"
#include "terminal/framebuffer.h"
#include "terminal/pty.h"
#include "graphic/screen.h"
#include "graphic/audio.h"

Terminal::Terminal(Framebuffer& fb, PTY& pty, Options const& options)
	: fb(fb), pty(pty), options(options), active(true), escape_mode(false), 
	  encoding(""), inbuf((char*)calloc(4, 1)), inbuf_pos(0)
{
}


Terminal::~Terminal()
{
	if(!encoding.empty())
		iconv_close(cd_in);
}


void
Terminal::Input()
{
	fb.Flash(false);

	// read data from the PTY
	int i;

	while((i = pty.Get()) != PTY::NO_DATA)
	{
		if(i == EOF)
		{
			active = false;
			return;
		}

		const char c = (const char)i;
		if(escape_mode)
			InputEscapeChar(c);
		else
			InputChar(c);
	}
}


void
Terminal::InputChar(const char c)
{
	char cv;

	switch(c)
	{
	case 27: // ESC
		escape_mode = true;
		escape_sequence = "\033";
		break;
	case '\n': // new line
		fb.AdvanceCursorY();
		break;
	case '\r': // carriage return
		fb.CarriageReturn();
		break;
	case '\t': // tab
		fb.Tab();
		break;
	case '\a': // beep
		cout << "Beep!" << endl; // TODO
		{
			Audio a;
			a.Beep();
		}
		break;
	case '\b': // backspace
		fb.Backspace();
		break;
	default:
		cv = ConvertByte(c);
		if(cv)
			fb.Put(cv, false);
	}
}

void 
Terminal::InputEscapeChar(const char c)
{
	escape_sequence += c;
	if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
	{
		escape_mode = false;
		ExecuteEscapeSequence(escape_sequence);
	}
}


void 
Terminal::Resize(int new_w, int new_h)
{
	fb.Resize(new_w, new_h);
	pty.Resize(new_w, new_h);
}


void 
Terminal::Output(Screen& screen)
{
	if(screen.keyQueue.empty())
		return;

	// write data to the PTY
	int w, h, fs, ts_w, ts_h;
	int ch = screen.keyQueue[0];
	screen.keyQueue.pop_front();

	switch(ch)
	{
	case 0: // discard
		break;
	case RESIZE:
		w = screen.keyQueue[0];
		screen.keyQueue.pop_front();
		h = screen.keyQueue[0];
		screen.keyQueue.pop_front();
		fs = screen.keyQueue[0];
		screen.keyQueue.pop_front();
		screen.Resize(w, h, fs, ts_w, ts_h);
		Resize(ts_w, ts_h);
		break;
	case QUIT:
		active = false;
		break;
	default:
		KeyPressed(ch);
	}
}


void
Terminal::KeyPressed(int ch)
{
	pty.Send((const char)ch);
}


void 
Terminal::ExecuteEscapeSequence(string const& sequence)
{
	cerr << "warning: Unrecognized escape sequence: ";
	for(string::const_iterator c = sequence.begin(); c < sequence.end(); c++)
		if(*c == 27)
			cerr << "ESC";
		else if(*c < 32)
			cerr << "(" << (int)*c << ")";
		else
			cerr << *c;
	cerr << endl;
}


void 
Terminal::SetEncoding(string const& encoding)
{
	this->encoding = encoding;

	cd_in = iconv_open(encoding.c_str(), options.CurrentEncoding.c_str());
	if(cd_in == (iconv_t)-1)
	{
		if(errno == EINVAL)
			cerr << "conversion from " << options.CurrentEncoding <<
				" to " << encoding << " not available." << endl;
		else
			perror("iconv_open");
		this->encoding = "";

	}
}

char
Terminal::ConvertByte(const char c)
{
	if((unsigned char)c < 128)
	{
		inbuf_pos = 0;
		return c;
	}

	char* wrptr = (char*)calloc(8, 1);
	size_t sz = 8;
	inbuf[inbuf_pos++] = c;
	char* t = wrptr;
	size_t nconv = iconv(cd_in, &inbuf, &inbuf_pos, &wrptr, &sz);
	if(nconv == 0)
	{
		inbuf_pos = 0;
		return t[0];
	}
	else
		return 0;
}
