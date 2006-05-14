#include <stdio.h>
#include <ncurses.h>
#include "tty.h"
#include "layout.h"

void init_curses (void)
{
	initscr ();
	
#ifdef HAVE_ESCDELAY
	ESCDELAY = 200;
#endif /* HAVE_ESCDELAY */

	raw ();
	noecho ();
	keypad (stdscr, TRUE);
	nodelay (stdscr, FALSE);
	start_color();
	init_pair(1, COLOR_BLUE, COLOR_WHITE);
	bkgd(COLOR_PAIR(1));
	curs_set(0);
}
