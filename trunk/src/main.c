#include <stdio.h>
#include <ncurses.h>
#include <menu.h>
#include <string.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define EXIT_KEY 0x078  /* <x> */
#define RETURN 0x00A
#define HOMEPAGE_URL "www.hqp.dl.am"

char *file_choices[] = {
	"Open",
	"Close",
	"Export/Save as...",
	"Reload",
	"Print",
	"Exit"
};

char *view_choices[] = {
	"Find",
	"Find next",
	"Find previous",
	"Document info",
};

char *opts_choices[] = {
	"External Programs",
	"Language",
	"Keybindings",
};

char *help_choices[] = {
	"OOView homepage",
	"Documentation",
	"Copying",
	"About OOView",
};

ITEM **file_items;
ITEM **view_items;
ITEM **opts_items;
ITEM **help_items;

MENU *file_menu;
MENU *view_menu;
MENU *opts_menu;
MENU *help_menu;

WINDOW *main_win;
WINDOW *menu_bar;
WINDOW *status_bar;
WINDOW *file_win;
WINDOW *view_win;
WINDOW *opts_win;
WINDOW *help_win;

int n_choices[4];

void print_status_bar(char *text)
{
		werase(status_bar);
		wprintw(status_bar, text);
		wrefresh(status_bar);
		
}

int print_file (FILE *ovd_file)
{
		int file_input;

		main_win = subwin(stdscr,LINES-2,COLS,1,0);
		scrollok(main_win,TRUE);
						
		print_status_bar("File successfully opened");
		while ( (file_input=fgetc(ovd_file)) != EOF)
		{
					waddch(main_win,file_input);
		}
		wscrl(main_win,1);
		touchwin(main_win);
		wrefresh(main_win);
		return 0;
}

void init_screen(void)
{
	mvprintw((LINES/2)-3, (COLS/2)-28, "OOView - Prints out OpenDocuments (odt) on your terminal");
	mvprintw((LINES/2)-2  , (COLS/2)-9,  "The VERY ALPHA");
	mvprintw((LINES/2), (COLS/2)-10, "Press <x> to quit");
}

void end_curses(void)
{
		int i;
		
		for (i=0; i<n_choices[0]; ++i)
			free_item(file_items[i]);
		
		for (i=0; i<n_choices[1]; ++i)
			free_item(view_items[i]);
		
		for (i=0; i<n_choices[2]; ++i)
			free_item(opts_items[i]);
	
		for (i=0; i<n_choices[3]; ++i)
			free_item(help_items[i]);
	
		free_menu(file_menu);
		free_menu(view_menu);
		free_menu(opts_menu);
		free_menu(help_menu);
	
		endwin();
}

int main (int argc, char **argv)
{
	FILE *ovd_file;

	int c;
	int i;
	ITEM *cur_item;
	int cur_menu;
	char *cmd;
	bool action_performed; /* fuck java ;-) */
	bool file_printed = false;
	initscr();
	cbreak();
	noecho();
	start_color();
	keypad(stdscr,TRUE);

	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_WHITE);
	init_pair(3, COLOR_BLUE, COLOR_WHITE);
	init_pair(4, COLOR_BLACK, COLOR_WHITE);
	bkgd(COLOR_PAIR(1));
	curs_set(0);
	
	menu_bar = subwin(stdscr,1,COLS,0,0);
	wbkgd(menu_bar,COLOR_PAIR(2));
	waddstr(menu_bar,"File");
	wattron(menu_bar, COLOR_PAIR(3));
	waddstr(menu_bar,"<F1>");
	wattroff(menu_bar, COLOR_PAIR(3));
	wmove(menu_bar, 0, 19);
	waddstr(menu_bar,"View");
	wattron(menu_bar, COLOR_PAIR(3));
	waddstr(menu_bar,"<F2>");
	wattroff(menu_bar, COLOR_PAIR(3));
	wmove(menu_bar, 0, 34);
	waddstr(menu_bar,"Options");
	wattron(menu_bar, COLOR_PAIR(3));
	waddstr(menu_bar,"<F3>");
	wattroff(menu_bar, COLOR_PAIR(3));
	wmove(menu_bar, 0, 53);
	waddstr(menu_bar,"Help");
	wattron(menu_bar, COLOR_PAIR(3));
	waddstr(menu_bar,"<F4>");
	wattroff(menu_bar, COLOR_PAIR(3));
	
	status_bar = subwin(stdscr,1,COLS,LINES-1,0);
	wbkgd(status_bar,COLOR_PAIR(4));
	
	n_choices[0] = ARRAY_SIZE(file_choices);
	n_choices[1] = ARRAY_SIZE(view_choices);
	n_choices[2] = ARRAY_SIZE(opts_choices);
	n_choices[3] = ARRAY_SIZE(help_choices);
	file_items = (ITEM **)calloc(n_choices[0] + 1, sizeof(ITEM *));
	view_items = (ITEM **)calloc(n_choices[1] + 1, sizeof(ITEM *));
	opts_items = (ITEM **)calloc(n_choices[2] + 1, sizeof(ITEM *));
	help_items = (ITEM **)calloc(n_choices[3] + 1, sizeof(ITEM *));

	for (i=0; i<n_choices[0]; ++i)
		file_items[i] = new_item(file_choices[i], NULL); 
	
	for (i=0; i<n_choices[1]; ++i)
		view_items[i] = new_item(view_choices[i], NULL); 
	
	for (i=0; i<n_choices[2]; ++i)
		opts_items[i] = new_item(opts_choices[i], NULL); 
	
	for (i=0; i<n_choices[3]; ++i)
		help_items[i] = new_item(help_choices[i], NULL); 
	
	file_items[n_choices[0]] = (ITEM *)NULL;
	view_items[n_choices[1]] = (ITEM *)NULL;
	opts_items[n_choices[2]] = (ITEM *)NULL;
	help_items[n_choices[3]] = (ITEM *)NULL;

	file_menu = new_menu((ITEM **)file_items);
	view_menu = new_menu((ITEM **)view_items);
	opts_menu = new_menu((ITEM **)opts_items);
	help_menu = new_menu((ITEM **)help_items);

	set_menu_mark(file_menu, "");
	set_menu_mark(view_menu, "");
	set_menu_mark(opts_menu, "");
	set_menu_mark(help_menu, "");

	init_screen();
	
	refresh();

	while ((c = getch()) != EXIT_KEY)
	{
		action_performed = false;
		
		switch (c)
		{
			case KEY_F(1):
					cur_menu=1;
					break;
			case KEY_F(2):
					cur_menu=2;
					break;
			case KEY_F(3):
					cur_menu=3;
					break;
			case KEY_F(4):
					cur_menu=4;
					break;
			case KEY_UP:
					wscrl(main_win,1);
					break;
			case KEY_DOWN:
					wscrl(main_win,-1);
					break;
		}
		
		if (cur_menu == 1)
		{
					file_win = newwin(8,19,1,0);
					keypad(file_win,TRUE);
					box(file_win,0,0);
					set_menu_win(file_menu, file_win);
					set_menu_sub(file_menu, derwin(file_win,6,17,1,1));
					post_menu(file_menu);
					while (c = wgetch(file_win))
					{
						if (c == KEY_DOWN)
								menu_driver(file_menu, REQ_DOWN_ITEM);
						else if (c == KEY_UP)
								menu_driver(file_menu, REQ_UP_ITEM);
						else if (c == KEY_RIGHT)
						{
								cur_menu = 2;
								break;
						}
						else if (c == KEY_LEFT)
						{
								cur_menu = 4;
								break;
						}
						else if (c == RETURN)
						{
								cur_menu = 0;
								cmd = (char *)item_name(current_item(file_menu));
								action_performed = true;
								break;
						}
						else
						{
								cur_menu = 0;
								break;
						}

					}			

					unpost_menu(file_menu);
					touchwin(stdscr);
					wrefresh(stdscr);
		}
		if (cur_menu == 2)
		{
					view_win = newwin(6,15,1,19);
					keypad(view_win,TRUE);
					box(view_win,0,0);
					set_menu_win(view_menu, view_win);
					set_menu_sub(view_menu, derwin(view_win,4,13,1,1));
					post_menu(view_menu);
					while (c = wgetch(view_win))
					{
						if (c == KEY_DOWN)
								menu_driver(view_menu, REQ_DOWN_ITEM);
						else if (c == KEY_UP)
								menu_driver(view_menu, REQ_UP_ITEM);
						else if (c == KEY_RIGHT)
						{
								cur_menu = 3;
								break;
						}
						else if (c == KEY_LEFT)
						{
								cur_menu = 1;
								break;
						}
						else if (c == RETURN)
						{
								cur_menu = 0;
								cmd = (char *)item_name(current_item(view_menu));
								action_performed = true;
								break;
						}
						else
						{
								cur_menu = 0;
								break;
						}
					}
					
					unpost_menu(view_menu);
					touchwin(stdscr);
					refresh();
		}
		if (cur_menu == 3)
		{
					opts_win = newwin(5,19,1,34);
					keypad(opts_win,TRUE);
					box(opts_win,0,0);
					set_menu_win(opts_menu, opts_win);
					set_menu_sub(opts_menu, derwin(opts_win,3,17,1,1));
					post_menu(opts_menu);
					while (c = wgetch(opts_win))
					{
						if (c == KEY_DOWN)
								menu_driver(opts_menu, REQ_DOWN_ITEM);
						else if (c == KEY_UP)
								menu_driver(opts_menu, REQ_UP_ITEM);
						else if (c == KEY_RIGHT)
						{
								cur_menu = 4;
								break;
						}
						else if (c == KEY_LEFT)
						{
								cur_menu = 2;
								break;
						}
						else if (c == RETURN)
						{
								cur_menu = 0;
								cmd = (char *)item_name(current_item(opts_menu));
								action_performed = true;
								break;
						}
						else
						{
								cur_menu = 0;
								break;
						}
					}
					
					unpost_menu(opts_menu);
					touchwin(stdscr);
					refresh();
		}
		if (cur_menu == 4)
		{
					help_win = newwin(6,17,1,53);
					keypad(help_win,TRUE);
					box(help_win,0,0);
					set_menu_win(help_menu, help_win);
					set_menu_sub(help_menu, derwin(help_win,4,15,1,1));
					post_menu(help_menu);
					while (c = wgetch(help_win))
					{
						if (c == KEY_DOWN)
								menu_driver(help_menu, REQ_DOWN_ITEM);
						else if (c == KEY_UP)
								menu_driver(help_menu, REQ_UP_ITEM);
						else if (c == KEY_RIGHT)
						{
								cur_menu = 1;
								break;
						}
						else if (c == KEY_LEFT)
						{
								cur_menu = 3;
								break;
						}
						else if (c == RETURN)
						{
								cur_menu = 0;
								cmd = (char *)item_name(current_item(help_menu));
								action_performed = true;
								break;
						}
						else
						{
								cur_menu = 0;
								break;
						}
					}
					
					unpost_menu(help_menu);
					touchwin(stdscr);
					refresh();
		}


		
		
		if (action_performed)
		{

				if (!strcmp(cmd,"Open"))
				{
						char *str;
						print_status_bar("Enter a file: ");
						curs_set(1);
						echo();
						wgetstr(status_bar,str);
						curs_set(0);
						noecho();

						if (strstr(str,".ovd")!=NULL)
						{
								ovd_file = fopen(str,"r");
		
								if (ovd_file != NULL)
								{
										print_file(ovd_file);
										file_printed = true;
										fclose(ovd_file);
								}
								else
								{
										print_status_bar("File does not exist!");
								}
						}
						else
						{
								print_status_bar("Must be a OOView-file (ovd) or OpenDocument-file (odt)!");	
						}
					
				}
				if (!strcmp(cmd,"Close"))
				{
						werase(main_win);
						init_screen();
						wrefresh(main_win);
						file_printed = false;
				}
				if (!strcmp(cmd,"Reload"))
				{
						if (file_printed)
						{
								char *str;
								ovd_file = fopen(str,"r");
								if (ovd_file != NULL)
								{
										print_file(ovd_file);
										file_printed = true;
										fclose(ovd_file);
								}
								else
								{
										print_status_bar("File does not exist!");
								}

						}
						else
						{
								print_status_bar("No open file!");
						}
				}
				if (!strcmp(cmd,"OOView homepage"))
				{
						print_status_bar(HOMEPAGE_URL);
				}
						
				if (!strcmp(cmd,"Exit"))
				{
				end_curses();
						return 0;
				}
		}

	}

	end_curses();
	return 0;
}
