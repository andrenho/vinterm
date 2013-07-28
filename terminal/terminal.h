#ifndef TERMINAL_H
#define TERMINAL_H

#include <iconv.h>
#include <string>
using namespace std;

#include "graphic/audio.h"

class Framebuffer;
class Screen;
class PTY;
class Options;

class Terminal
{
public:
	Terminal(Framebuffer& fb, PTY& pty, Options const& options);
	virtual ~Terminal();

	void Input();
	void Output(Screen& screen);
	void Resize(int new_w, int new_h);
	void SetEncoding(string const& encoding);
	void SendString(string s);

	virtual void ExecuteEscapeSequence(string const& sequence);
	virtual void KeyPressed(uint32_t key);

	bool Active() const { return active; }

protected:
	Framebuffer& fb;
	PTY& pty;
	Options const& options;

private:
	void InputChar(const char c);
	void InputEscapeChar(const char c);
	char ConvertByteInput(const char c);

	Audio const* audio;

	bool active;
	bool escape_mode;
	string escape_sequence;
	string encoding;

	iconv_t cd_in, cd_out;
	char *inbuf, *original_inbuf;
	size_t inbuf_pos;
};

#endif
