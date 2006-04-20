#include <curses.h>
#include <stdlib.h>

/*ASCII-Codes*/
#define ENTER 10
#define ESCAPE 27
/*Number of the Menuitems*/
#define MENU1 6
#define MENU2 4
#define MENU3 3
#define MENU4 4

int convert_to_ovd(char** path, char** destpath);
int convert_to_txt(char **path, char **destpath);
int read_ovd(char** path);
int export_file(char** path, char** destpath);
int load_cfg();
int print(char **path);
/*int view_gfx(char** path);*/
int view_gfx(char** path, char** alt_text);
int load_lang(char** path);
int load_keyb();
int olog(int err);




void init_curses () {
	initscr();
	start_color();
	init_pair(1,COLOR_WHITE,COLOR_BLACK);
	init_pair(2,COLOR_BLUE,COLOR_WHITE);
	init_pair(3,COLOR_RED,COLOR_WHITE);
	curs_set(0);
	noecho();
	raw();
	keypad(stdscr,TRUE);
}

void draw_menubar (WINDOW *menubar) {
	wbkgd(menubar,COLOR_PAIR(2));
	
	waddstr(menubar, "File");
	wattron(menubar, COLOR_PAIR(3));
	waddstr(menubar,"<F1>");	
	wattroff(menubar,COLOR_PAIR(3));
	
	wmove(menubar,0,10);
	waddstr(menubar,"View");
	wattron(menubar,COLOR_PAIR(3));
	waddstr(menubar,"<F2>");
	wattroff(menubar, COLOR_PAIR(3));
	
	wmove(menubar,0,20);
	waddstr(menubar, "Options");
	wattron(menubar, COLOR_PAIR(3));
	waddstr(menubar, "<F3>");
	wattroff(menubar, COLOR_PAIR(3));
	
	wmove(menubar,0,33);
	waddstr(menubar, "Help");
	wattron(menubar,COLOR_PAIR(3));
	waddstr(menubar,"<F4>");
	wattroff(menubar,COLOR_PAIR(3));
}

WINDOW **draw_menu (int start_col) {
	int i;
	WINDOW **items;
	if (start_col == 0) { /*menu FILE*/
		items = (WINDOW **)malloc(MENU1*sizeof(WINDOW *));

		items[0] = newwin (MENU1+2,22,1,start_col);
		wbkgd (items[0],COLOR_PAIR(2));
		box (items[0],ACS_VLINE,ACS_HLINE);
		for (i=1; i<=MENU1; i++)
			items[i] = subwin (items[0],1,20,i+1,start_col+1);
		wprintw (items[1],"Open");
		wprintw (items[2],"Close");
		wprintw (items[3],"Export / Save as...");
		wprintw (items[4],"Reload");
		wprintw (items[5],"Print");
		wprintw (items[6],"Exit");
		wbkgd (items[1],COLOR_PAIR(1));
		wrefresh(items[0]);
	} else if (start_col == 10)  { /*menu VIEW*/
		items = (WINDOW **)malloc(MENU2*sizeof(WINDOW *));

		items[0] = newwin (MENU2+2,17,1,start_col);
		wbkgd (items[0],COLOR_PAIR(2));
		box (items[0],ACS_VLINE,ACS_HLINE);
		for (i=1;i<=MENU2;i++)
			items[i] = subwin (items[0],1,15,i+1,start_col+1);
		wprintw (items[1],"Find");
		wprintw (items[2],"Find next");
		wprintw (items[3],"Find previous");
		wprintw (items[4],"Document info");
		wbkgd (items[1],COLOR_PAIR(1));
		wrefresh(items[0]);
	} else if (start_col == 20)	{ /*menu OPTIONS*/
		items = (WINDOW **)malloc(MENU3*sizeof(WINDOW *));

		items[0] = newwin (MENU3+2,21,1,start_col);
		wbkgd (items[0],COLOR_PAIR(2));
		box (items[0],ACS_VLINE,ACS_HLINE);
		for (i=1;i<=MENU3;i++)
			items[i] = subwin (items[0],1,19,i+1,start_col+1);
		wprintw (items[1],"External Programs");
		wprintw (items[2],"Language");
		wprintw (items[3],"Keybindings");
		wbkgd (items[1],COLOR_PAIR(1));
		wrefresh(items[0]);
	} else if (start_col == 33)  { /*menu HELP*/
		items = (WINDOW **)malloc(MENU4*sizeof(WINDOW *));

		items[0] = newwin (MENU4+2,19,1,start_col);
		wbkgd (items[0],COLOR_PAIR(2));
		box (items[0],ACS_VLINE,ACS_HLINE);
		for (i=1;i<=MENU4;i++)
			items[i] = subwin (items[0],1,17,i+1,start_col+1);
		wprintw (items[1],"ooview Homepage");
		wprintw (items[2],"Documentation");
		wprintw (items[3],"Copying");
		wprintw (items[4],"About ooview");
		wbkgd (items[1],COLOR_PAIR(1));
		wrefresh(items[0]);
	}
	return items;
}

void delete_menu (WINDOW **items, int count) {
	int i;
	for (i=0;i<count;i++)
		delwin(items[i]);
	free(items);
}

int scroll_menu (WINDOW **items, int count, int menu_start_col) {
	int key;
	int selected = 0;

	while (1) {
		key = getch();
		if (key == KEY_DOWN || key == KEY_UP) {
			wbkgd (items[selected+1],COLOR_PAIR(2));
			wnoutrefresh (items[selected+1]);
			if (key == KEY_DOWN) {
				selected = (selected + 1) % count;
			} else {
				selected = (selected + count - 1) % count;
			}
			wbkgd (items[selected+1],COLOR_PAIR(1));
			wnoutrefresh (items[selected+1]);
			doupdate();
		} else if (key == KEY_LEFT || key == KEY_RIGHT) {
			delete_menu (items,count+1);
			touchwin (stdscr);
			refresh();
			items = draw_menu (10-menu_start_col);
			return scroll_menu(items,3,menu_start_col);
		} else if (key == ESCAPE) {
			return -1;
		} else if (key == ENTER) {
			return selected;
		}
	}
}


int main (int argc, char **argv) {
	int key;
	WINDOW *menubar;
	WINDOW *messagebar;
	int i;
	
	init_curses();
	bkgd(COLOR_PAIR(1));
	menubar = subwin(stdscr,1,COLS,0,0);
	draw_menubar(menubar);
	messagebar = subwin(stdscr,1,COLS,LINES,0);
	refresh();
	
	do {
		int selected_item;
		WINDOW **menu_items;
		
		wclear(messagebar);
		wrefresh(messagebar);
		wclear(menubar);
		draw_menubar(menubar);
		if (argc == 1) {
			move ((LINES/2)-1,(COLS/2)-34);
			printw("OOView - Prints out OpenOffice.org documents (.odt) on your terminal");
			move(LINES/2+1,(COLS/2)-18);
			printw("Press <F1> or <F2> to open the menus.");
			move((LINES/2)+2,(COLS/2)-10);
			printw("Press <ESC> to quit.");
			refresh();
		}
		key = getch();
		if (key == KEY_F(1)) {
			menu_items = draw_menu(0);
			selected_item = scroll_menu (menu_items, MENU1, 0);
			/*delete_menu(menu_items,4);*/
			touchwin(stdscr);
			refresh();
		}
		if (key == KEY_F(2)) {				
			menu_items = draw_menu(10);
			selected_item = scroll_menu (menu_items, MENU2, 10);
			/*delete_menu(menu_items,4);*/
			touchwin(stdscr);
			refresh();
		}
		if (key == KEY_F(3)) {
			menu_items = draw_menu(20);
			selected_item = scroll_menu (menu_items, MENU3, 20);
			/*delete_menu(menu_items,4);*/
			touchwin(stdscr);
			refresh();	
		}
		if (key == KEY_F(4)) {
			menu_items = draw_menu(30);
			selected_item = scroll_menu (menu_items, MENU4, 30);
			/*delete_menu(menu_items,4);*/
			touchwin(stdscr);
			refresh();	
		}
		wprintw(stdscr,"YES");
		clear();
	} while (key != ESCAPE);
	
	delwin(menubar);
	delwin(messagebar);
	endwin();
	return EXIT_SUCCESS;
}

