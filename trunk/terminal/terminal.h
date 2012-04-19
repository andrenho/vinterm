#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>
using namespace std;

class Framebuffer;
class Screen;
class PTY;

class Terminal
{
public:
	Terminal(Framebuffer& fb, PTY& pty);
	virtual ~Terminal();

	void Input();
	void Output(Screen& screen);
	void Resize(int new_w, int new_h);
	void SetEncoding(string const& encoding);

	virtual void ExecuteEscapeSequence(string const& sequence);
	virtual void KeyPressed(int key);

	bool Active() const { return active; }

protected:
	Framebuffer& fb;
	PTY& pty;

private:
	void InputChar(const char c);
	void InputEscapeChar(const char c);

	bool active;
	bool escape_mode;
	string escape_sequence;
};

#endif
