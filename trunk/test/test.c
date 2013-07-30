#include <ncurses.h>

int main()
{
	MEVENT event;

	initscr();
	start_color();
	raw();
	keypad(stdscr, TRUE);
	noecho();

	// attributes
	attrset(A_NORMAL); printw("A_NORMAL\n");
	attrset(A_STANDOUT); printw("A_STANDOUT\n");
	attrset(A_UNDERLINE); printw("A_UNDERLINE\n");
	attrset(A_BLINK); printw("A_BLINK\n");
	attrset(A_DIM); printw("A_DIM\n");
	attrset(A_BOLD); printw("A_BOLD\n");
	attrset(A_PROTECT); printw("A_PROTECT\n");
	attrset(A_INVIS); printw("A_INVIS\n");
	attrset(A_ALTCHARSET); printw("A_ALTCHARSET\n");
	attrset(A_CHARTEXT); printw("A_CHARTEXT\n");
	init_pair(1, COLOR_CYAN, COLOR_RED);
	attrset(COLOR_PAIR(1)); printw("COLOR_PAIR\n");

	// mouse
	mousemask(ALL_MOUSE_EVENTS, NULL);

	// ending
	attrset(A_NORMAL);
	mvprintw(23, 1, "Press F10 to end");
	refresh();

	// press F10
	int ch;
	while((ch = getch()) != KEY_F(10)) 
	{ 
		if(ch == KEY_MOUSE)
			if(getmouse(&event) == OK)
				break;
	}

	endwin();

	return 0;
}
