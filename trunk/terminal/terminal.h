#ifndef TERMINAL_H
#define TERMINAL_H

#include <iconv.h>
#include <string>
using namespace std;

#include "global.h"

class Terminal
{
public:
	Terminal();
	virtual ~Terminal();

	void Input();
	void Output();
	void Resize(int new_w, int new_h);
	void SetEncoding(string const& encoding);
	void SendString(string s);

	virtual void ExecuteEscapeSequence(string const& sequence);
	virtual void KeyPressed(uint32_t key);

	bool Active() const { return active; }

protected:
	bool alternateCharset;
	bool readingStatusLine;

private:
	bool InputChar(const char c);
	virtual void InputAlternateChar(const char c);
	void InputEscapeChar(const char c);
	char ConvertByteInput(const char c);

	bool active;
	bool escape_mode;
	string escape_sequence;
	string encoding;

	iconv_t cd_in, cd_out;
	char *inbuf, *original_inbuf;
	size_t inbuf_pos;
};

#endif
