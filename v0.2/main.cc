#include <string>
#include <iostream>
using namespace std;

#include "options.h"
#include "graphic/screen.h"
#include "terminal/pty.h"
#include "terminal/framebuffer.h"
#include "terminal/vinterm.h"

int main(int argc, char** argv)
{
	/* Read and initialize options from the program arguments. In the
	 * future, will also read from a configuration file. */
	Options options(argc, argv);

	/* Initialize the framebuffer. The framebuffer is a (initially) 80x25
	 * grid of characters and their attributes, and represents the console
	 * that the user sees. */
	Framebuffer fb(options);

	/* The PTY is the class that connects to the virtual terminal. It
	 * receives the characters sent by the terminal, and sends the chars
	 * inputted by the user to the terminal. */
	PTY pty(options.debug_terminal);

	/* The terminal is the class that connects the PTY with the framebuffer.
	 * It reads the data from the PTY and stores it on the framebuffer,
	 * translating the escape codes from the specific terminal on demand. */
	Vinterm terminal(fb, pty);

	/* Opens the screen that the user will interact with. It reads the
	 * characters from the framebuffer and display them on the screen, using
	 * transforming them in the 80s style. */
	Screen screen(options, fb);

	/* The main loop is very simple. It executes these steps continually:
	 *
	 *   1. Read data from the terminal (Input);
	 *   2. Read keys pressed by the user and send them to the terminal
	 *      (Output);
	 *   3. Update everything in the screen for the user.
	 *
	 * The loop exits when a EOF is received from the terminal. */
	while(terminal.Active())
	{
		terminal.Input();
		terminal.Output(screen);
		screen.Update();
	}

	return 0;
}
