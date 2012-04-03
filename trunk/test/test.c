#include <curses.h>

int main()
{
	initscr();
	keypad(stdscr, TRUE);

	refresh();
	int i = getch() == KEY_NPAGE;
	endwin();

	printf("%d\n", i);
	return 0;
}
