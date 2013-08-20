#include "terminal/terminal.h"

#include <iostream>
using namespace std;

#include "options.h"
#include "terminal/charmatrix.h"
#include "terminal/pty.h"
#include "terminal/keyqueue.h"
#include "graphic/audio.h"
#include "graphic/screen.h"

Terminal::Terminal(CharMatrix& cm, PTY& pty, Options const& options)
	: cm(cm), pty(pty), options(options), alternateCharset(false),
	  readingStatusLine(false), audio(new Audio(options)), active(true), 
	  escape_mode(false), encoding(""), inbuf((char*)calloc(4, 1)), 
	  original_inbuf(inbuf), inbuf_pos(0)
{
	mouse.setTerminal(this);
}


Terminal::~Terminal()
{
	if(!encoding.empty())
	{
		iconv_close(cd_in);
		iconv_close(cd_out);
	}
	free(original_inbuf);
	delete audio;
}


void 
Terminal::SendString(string s)
{
	pty.Send(s);
}


void
Terminal::Input()
{
	cm.Flash(false);

	// read data from the PTY
	int i;

	while((i = pty.Get()) != PTY::NO_DATA)
	{
		if(i == EOF)
		{
			active = false;
			return;
		}

		cm.ForetrackToScreen();

		const char c = (const char)i;
		bool update_scr = false;
		if(escape_mode)
			InputEscapeChar(c);
		else
			update_scr = InputChar(c);

		if(update_scr)
			return;
	}
}


bool
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
		return cm.AdvanceCursorY();
	case '\r': // carriage return
		cm.CarriageReturn();
		break;
	case '\t': // tab
		cm.Tab();
		break;
	case '\a': // beep
		audio->Beep();
		break;
	case '\b': // backspace
		cm.Backspace();
		break;
	default:
		if(alternateCharset)
			InputAlternateChar(c);
		else
		{
			cv = ConvertByteInput(c);
			if(cv) 
			{
				if(readingStatusLine)
				{
					string s = cm.terminalTitle();
					cm.setTerminalTitle(s + cv);
				}
				else
					cm.Put(cv, false);
			}
		}
	}

	return false;
}


void
Terminal::InputAlternateChar(const char c)
{
	cm.Put(c, false);
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
	cm.Resize(new_w, new_h);
	pty.Resize(new_w, new_h);
}


void 
Terminal::Output(Screen& screen)
{
	while(!keyQueue.empty())
	{
		// write data to the PTY
		int w, h, fs, ts_w, ts_h;
		string s;
		uint32_t ch = keyQueue[0];
		keyQueue.pop_front();

		switch(ch)
		{
		case 0: // discard
			break;
		case SH_PAGE_UP:
			cm.BackTrack();
			break;
		case SH_PAGE_DOWN:
			cm.ForeTrack();
			break;
		case RESIZE:
			w = keyQueue[0];
			keyQueue.pop_front();
			h = keyQueue[0];
			keyQueue.pop_front();
			fs = keyQueue[0];
			keyQueue.pop_front();
			screen.Resize(w, h, fs, ts_w, ts_h);
			Resize(ts_w, ts_h);
			break;
		case MPRESS:
		case MRELEASE:
		case MDRAG:
			s = mouse.Translate(ch);
			if(!s.empty())
				SendString(s);
			break;
		case QUIT:
			active = false;
			break;
		default:
			KeyPressed(ch);
		}
	}
}


void
Terminal::KeyPressed(uint32_t ch)
{
	size_t ibl = 2; // len of iso
	size_t obl = 5; // len of converted

	// if screen is rolled back, restore
	cm.ForetrackToScreen();

	char *iso = (char*)calloc(ibl, sizeof(char));
	char *iso_ptr = iso;
	iso[0] = (char)ch;
	char *converted = (char*)calloc(obl, sizeof(char));
	char *converted_start = converted;

	size_t ret = iconv(cd_out, &iso, &ibl, &converted, &obl);

	if(ret == (size_t)-1)
	{
		perror("iconv");
		return;
	}
	else
	{
		int i = 0;
		while(converted_start[i])
			pty.Send(converted_start[i++]);
	}

	free(iso_ptr);
	free(converted_start);
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
		return;
	}

	cd_out = iconv_open(options.CurrentEncoding.c_str(), "ISO-8859-1");
	if(cd_out == (iconv_t)-1)
	{
		if(errno == EINVAL)
			cerr << "conversion from ISO-8859-1 to " << 
				options.CurrentEncoding.c_str() <<
				" not available." << endl;
		else
			perror("iconv_open");
		this->encoding = "";
		return;
	}
}

char
Terminal::ConvertByteInput(const char c)
{
	if((unsigned char)c < 128)
	{
		inbuf_pos = 0;
		return c;
	}

	char* wrptr = (char*)calloc(8, 1);
	char* free_me = wrptr;
	size_t sz = 8;
	if(inbuf_pos >= 3)
		abort();
	original_inbuf[inbuf_pos++] = c;
	inbuf = original_inbuf;
	char* t = wrptr;
	size_t nconv = iconv(cd_in, &inbuf, &inbuf_pos, &wrptr, &sz);
	if(nconv == 0)
	{
		inbuf_pos = 0;
		char ch = t[0];
		free(free_me);
		return ch;
	}
	else
	{
		if(errno == EINVAL)
		{
			free(free_me);
			return 0; // incomplete sequence, will read next char
		}
		else
		{
			free(free_me);
			inbuf_pos = 0;
			return (char)254; // complete but invalid sequence
		}
	}
}
