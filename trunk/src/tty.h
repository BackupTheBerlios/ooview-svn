#ifndef __TTY_H
#define __TTY_H

// This file takes car of loading ncurses

#ifdef USE_NCURSES
#	ifdef HAVE_NCURSES_CURSES_H
#		include <ncurses/curses.h>
#	elif HAVE_NCURSES_H
#		include <ncurses.h>
#	else
#		include <curses.h>
#	endif
#endif

#endif
