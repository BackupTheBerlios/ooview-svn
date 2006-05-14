/*
 * Copyright (C) 2006 ooview
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */


#define ESCAPE 0x1B
#define RETURN 0xA

#include <stdlib.h>
#include <ncurses.h>

#include "global.h"
#include "tty.h"
#include "menu.h"
#include "main.h"
#include "cmd.h"
#include "boxes.h"
#include "layout.h"
#include "dialog.h"


// The Menubar
struct WMenu *the_menubar = NULL;

static menu_entry FileMenu[] = {
	{' ',"&Open               Ctrl-O", 'O', open_box},
	{' ',"&Close              Ctrl-W", 'C', close_cmd},
	{' ',"&Export/Save as...  Ctrl-S", 'E', export_box},
	{' ',"&Reload                 F5", 'R', reload_cmd},
	{' ',"&Print              Ctrl-P", 'P', print_cmd},
	{' ',"e&Xit               Ctrl-Q", 'X', quit_cmd}
};

static menu_entry ViewMenu[] = {
	{' ',"&Find                    /", 'F', find_box},
	{' ',"Find &next               n", 'N', find_next_cmd},
	{' ',"Find &previous           p", 'P', find_prev_cmd},
	{' ',"&Document info           ?", 'D', doc_info_box}
};

static menu_entry OptMenu[] = {
	{' ',"&External Programs", 'E', ext_progs_box},
	{' ',"&Language", 'L', language_box},
	{' ',"&Keybindings", 'K', keybindings_box}
};

static menu_entry HelpMenu[] = {
	{' ',"ooview &Homepage          ", 'H'/*, homepage_box*/},
	{' ',"&Documentation          F1", 'D'/*, documentation_box*/},
	{' ',"&Copying                  ", 'C'/*, copying_box*/},
	{' ',"&About ooview             ", 'A'/*, about_box*/}
};

#define menu_entries(x) sizeof(x)/sizeof(menu_entry)

static Menu *MenuBar[4];

void init_menu (void)
{
    MenuBar[0] = create_menu (" &File ", FileMenu, menu_entries (FileMenu), "[File Menu]");
    MenuBar[1] = create_menu (" &View ", ViewMenu, menu_entries (ViewMenu), "[View Menu]");
    MenuBar[2] = create_menu (" &Options ", OptMenu, menu_entries (OptMenu), "[Options Menu]");
    MenuBar[3] = create_menu (" &Help ", HelpMenu, menu_entries (HelpMenu), "[Help Menu]");
}

/*void done_menu (void)
{
    int i;

    for (i = 0; i < 5; i++)
	{
		destroy_menu (MenuBar[i]);
	}
}*/

int main(int argc, char **argv)
{
	int ch;
	init_curses();
	
	if (argc = 1)
	{
		mvaddstr((LINES/2)-1,(COLS/2)-34,"OOView - Prints out OpenOffice.org documents (.odt) on your terminal");
		mvaddstr((LINES/2),(COLS/2)-7,"The VERY BETA");
		mvaddstr((LINES/2)+2,(COLS/2)-10,"Press <ESC> to quit");
	}
	while ((ch=getch()) != KEY_F(1))
	{
	}
	
	endwin();
	return EXIT_SUCCESS;
}

