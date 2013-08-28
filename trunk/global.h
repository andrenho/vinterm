#ifndef GLOBAL_H
#define GLOBAL_H

class Options;
class CharMatrix;
class PTY;
class Terminal;
class Font;
class Mouse;
class Framebuffer;
class Screen;
class Renderer;
class Audio;
class Chronometer;

extern Options* 	options;
extern CharMatrix* 	cm;
extern PTY* 		pty;
extern Terminal*	terminal;
extern Font*		font;
extern Mouse*		mouse;
extern Framebuffer*	framebuffer;
extern Screen*		screen;
extern Renderer*	renderer;
extern Audio*		audio;
extern Chronometer*	ch;

#endif
