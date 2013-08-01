#include <ncurses.h>

int main()
{
	MEVENT e;

	initscr();
	clear();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);

	mouseinterval(0);
	mousemask(ALL_MOUSE_EVENTS, NULL);

	bool end = false;
	while(!end)
	{
		int c = getch();
		switch(c)
		{
		case 10:
			end = true;
			break;
		case KEY_MOUSE:
			if(getmouse(&e) == OK)
			{
				printw("id: %d, x: %d, y: %d, z: %d\n",
						e.id, e.x, e.y, e.z);

				if(e.bstate & BUTTON1_PRESSED)
					printw("BUTTON1_PRESSED\n");
				if(e.bstate & BUTTON1_RELEASED)
					printw("BUTTON1_RELEASED\n");
				if(e.bstate & BUTTON1_CLICKED)
					printw("BUTTON1_CLICKED\n");
				if(e.bstate & BUTTON1_DOUBLE_CLICKED)
					printw("BUTTON1_DOUBLE_CLICKED\n");
				if(e.bstate & BUTTON1_TRIPLE_CLICKED)
					printw("BUTTON1_TRIPLE_CLICKED\n");
				if(e.bstate & BUTTON2_PRESSED)
					printw("BUTTON2_PRESSED\n");
				if(e.bstate & BUTTON2_RELEASED)
					printw("BUTTON2_RELEASED\n");
				if(e.bstate & BUTTON2_CLICKED)
					printw("BUTTON2_CLICKED\n");
				if(e.bstate & BUTTON2_DOUBLE_CLICKED)
					printw("BUTTON2_DOUBLE_CLICKED\n");
				if(e.bstate & BUTTON2_TRIPLE_CLICKED)
					printw("BUTTON2_TRIPLE_CLICKED\n");
				if(e.bstate & BUTTON3_PRESSED)
					printw("BUTTON3_PRESSED\n");
				if(e.bstate & BUTTON3_RELEASED)
					printw("BUTTON3_RELEASED\n");
				if(e.bstate & BUTTON3_CLICKED)
					printw("BUTTON3_CLICKED\n");
				if(e.bstate & BUTTON3_DOUBLE_CLICKED)
					printw("BUTTON3_DOUBLE_CLICKED\n");
				if(e.bstate & BUTTON3_TRIPLE_CLICKED)
					printw("BUTTON3_TRIPLE_CLICKED\n");
				if(e.bstate & BUTTON4_PRESSED)
					printw("BUTTON4_PRESSED\n");
				if(e.bstate & BUTTON4_RELEASED)
					printw("BUTTON4_RELEASED\n");
				if(e.bstate & BUTTON4_CLICKED)
					printw("BUTTON4_CLICKED\n");
				if(e.bstate & BUTTON4_DOUBLE_CLICKED)
					printw("BUTTON4_DOUBLE_CLICKED\n");
				if(e.bstate & BUTTON4_TRIPLE_CLICKED)
					printw("BUTTON4_TRIPLE_CLICKED\n");
				if(e.bstate & BUTTON_SHIFT)
					printw("BUTTON_SHIFT\n");
				if(e.bstate & BUTTON_CTRL)
					printw("BUTTON_CTRL\n");
				if(e.bstate & BUTTON_ALT)
					printw("BUTTON_ALT\n");
			}
			break;
		}
	}

	clrtoeol();
	refresh();
	endwin();
}
