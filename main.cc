#include <string>
#include <iostream>
using namespace std;

#include "options.h"
#include "graphic/framebuffer.h"
#include "graphic/screen.h"
#include "render/monochrome.h"
#include "terminal/pty.h"
#include "terminal/charmatrix.h"
#include "terminal/vinterm.h"
#include "terminal/clipboard.h"

int main(int argc, char** argv)
{
	/* Read and initialize options from the program arguments. In the
	 * future, will also read from a configuration file. */
	Options options(argc, argv);

	/* Initialize the charmatrix. The charmatrix is a (initially) 80x25
	 * grid of characters and their attributes, and represents the console
	 * that the user sees. */
	CharMatrix cm(options);

	/* The PTY is the class that connects to the virtual terminal. It
	 * receives the characters sent by the terminal, and sends the chars
	 * inputted by the user to the terminal. */
	PTY pty(options);

	/* The terminal is the class that connects the PTY with the charmatrix.
	 * It reads the data from the PTY and stores it on the charmatrix,
	 * translating the escape codes from the specific terminal on demand. */
	Vinterm terminal(cm, pty, options);

	/* Opens the screen that the user will interact with. It reads the
	 * characters from the charmatrix and draws the pixels. */
	Framebuffer framebuffer(options, cm);

	/* Create a renderer. */
	Monochrome monochrome;

	/* The screen reads data from the pixes and display them on the screen, 
	 * transforming them in the 80s style. */
	Screen screen(options, framebuffer, monochrome, terminal.mouse);

	/* Now that the font was loaded (in Screen), set up the terminal
	   encoding. */
//	terminal.SetEncoding(screen.FontEncoding());

	/* Initialize clipboard. */
	cm.clipboard.ConnectToWM();

	/* The main loop is very simple. It executes these steps continually:
	 *
	 *   1. Read data from the terminal (Input);
	 *   2. Read keys pressed by the user and send them to the terminal
	 *      (Output);
	 *   3. Update everything in the screen for the user;
	 *   4. Check for events.
	 *
	 * The loop exits when a EOF is received from the terminal. */
	//terminal.SendString(options.toBeRun);
	while(terminal.Active())
	{
		terminal.Output(screen);
		terminal.Input();
		screen.Update();
		screen.CheckEvents();
	}

	return 0;
}
