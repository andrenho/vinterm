#include <string>
#include <iostream>
using namespace std;

#include "global.h"
#include "options.h"
#include "terminal/charmatrix.h"
#include "terminal/pty.h"
#include "terminal/vinterm.h"
#include "terminal/mouse.h"
#include "graphic/font.h"
#include "graphic/framebuffer.h"
#include "graphic/screen.h"
#include "graphic/audio.h"
#include "render/simple.h"

Options* 	options		= nullptr;
CharMatrix* 	cm		= nullptr;
PTY* 		pty		= nullptr;
Terminal*	terminal 	= nullptr;
Mouse*          mouse		= nullptr;
Font*		font		= nullptr;
Framebuffer*	framebuffer	= nullptr;
Screen*		screen 		= nullptr;
Renderer*	renderer 	= nullptr;
Audio*		audio 		= nullptr;

int main(int argc, char** argv)
{
	/* Read and initialize options from the program arguments. In the
	 * future, will also read from a configuration file. */
	options = new Options(argc, argv);

	/* Initialize the charmatrix. The charmatrix is a (initially) 80x25
	 * grid of characters and their attributes, and represents the console
	 * that the user sees. */
	cm = new CharMatrix();

	/* The PTY is the class that connects to the virtual terminal. It
	 * receives the characters sent by the terminal, and sends the chars
	 * inputted by the user to the terminal. */
	pty = new PTY();

	/* The terminal is the class that connects the PTY with the charmatrix.
	 * It reads the data from the PTY and stores it on the charmatrix,
	 * translating the escape codes from the specific terminal on demand. */
	terminal = new Vinterm();

	/* Initialize the mouse code translation. */
	mouse = new Mouse();

	/* Load the font defined by the user. */
	try
	{
		font = Font::LoadFont();
	}
	catch(string s)
	{
		cerr << s << endl;
		return 1;
	}

	/* Opens the screen that the user will interact with. It reads the
	 * characters from the charmatrix and draws the pixels. */
	framebuffer = new Framebuffer();

	/* The screen reads data from the pixes and display them on the screen, 
	 * transforming them in the 80s style. */
	screen = new Screen();

	/* Create a renderer. */
	renderer = new Simple();

	/* Initialize audio. */
	audio = new Audio();

	/* Now that the font was loaded, set up the terminal encoding. */
	terminal->SetEncoding(font->Encoding());

	/* Initialize clipboard. */
	cm->clipboard.ConnectToWM();

	/* The main loop is very simple. It executes these steps continually:
	 *
	 *   1. Read data from the terminal (Input);
	 *   2. Read keys pressed by the user and send them to the terminal
	 *      (Output);
	 *   3. Update everything in the screen for the user;
	 *   4. Check for events.
	 *
	 * The loop exits when a EOF is received from the terminal. */
	terminal->SendString(options->toBeRun);
	while(terminal->Active())
	{
		terminal->Output();
		terminal->Input();
		framebuffer->DrawChars();
		screen->Update();
		cm->CheckForBlink();
		screen->CheckEvents();
	}

	/* Clean up. */
	delete audio;
	delete renderer;
	delete screen;
	delete framebuffer;
	delete font;
	delete mouse;
	delete terminal;
	delete pty;
	delete cm;
	delete options;

	return 0;
}
