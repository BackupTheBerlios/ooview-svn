#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <menu.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h> /*for logging*/

#include "metaparser.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define EXIT_KEY 0x078  /* <x> */
#define RETURN 0x00A
#define NEWLINE 10
#define HOMEPAGE_URL "http://ooview.berlios.de"

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

char *configpath="/etc/ooview/config";

char *returnvalues [] = {
	"success",
	"odt file not found",
	"ovd file not found",
	"could not open odt file",
	"could not open ovd file",
	"could not open txt file",
	"odt file corruption",
	"ovd file corruption",
	"system call returns not 0",
	"could not write log file. ironic",
	"language file not found",
	"no external program defined",
	"unknown error. should not happen :("
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
WINDOW *meta_win;
WINDOW *file_win;
WINDOW *view_win;
WINDOW *opts_win;
WINDOW *help_win;

FILE *ovd_file;
FILE *odt_file;
ITEM *cur_item;
struct fileinfo *buffer;
char *cur_char;
int file_type;
int c;
int i;
int cur_menu = 0;
char *cmd;
bool action_performed; /* fuck java ;-) */
bool file_printed = false;
	


#define clear_status_bar() print_status_bar("");

void print_status_bar(char *text)
{
		werase(status_bar);
		wprintw(status_bar, text);
		touchwin(status_bar);
		wrefresh(status_bar);	
}

void print_site (int cur_line, int lines)
{
		werase(status_bar);
		mvwprintw(status_bar,0,(COLS/2),"%d - %d",cur_line,lines);
		touchwin(status_bar);
		wrefresh(status_bar);
}

void open_odt (char *filename, struct fileinfo *buffer)
{
		FILE *tmp;
		char unzip_cmd[255] = "unzip -qq -u -d /tmp/ooview ";
		char syscall[255] = "cat /tmp/ooview/content.xml | o3totxt > /tmp/ooview/content.tmp";
		
		system("rm -rf /tmp/ooview");
		strcat(unzip_cmd,filename);
		/*strcat(unzip_cmd," -qq");*/
		printf("%s",unzip_cmd);
		system(unzip_cmd);
		
		system(syscall);
		
		tmp = fopen ("/tmp/ooview/content.tmp","r");
		get_file_content(tmp, filename, buffer);
		fclose(tmp);
		
}

void get_file_content (FILE *file, char *filename, struct fileinfo *buffer)
{
		char *file_input;
		struct stat attribut;
		stat(filename,&attribut);
		buffer->lines = 0;
		buffer->cur_line = 1;
		
		file_input = (char *)malloc(COLS*sizeof(char));
		buffer->content = (char *)malloc(attribut.st_size);
	
		werase(main_win);
		
		while ( (fgets(file_input,COLS+1,file)) != NULL)
		{
				buffer->lines++;
				strcat(buffer->content,file_input);
		}
}

int print_file (struct fileinfo *buffer, char *cur_char)
{	
		int output;
		int i = 0;
		int lines_to_write = COLS;
				
		werase(main_win);

		do {
				i = 0;
				
				do {
						output = *cur_char;
						if (output==0)
								break;
						waddch(main_win,output);
						cur_char++;
						i++;
				} while ((output!=NEWLINE) && (i<COLS));
				lines_to_write--;
		} while ((lines_to_write!=0) && (output!=0));
	
		touchwin(main_win);
		wrefresh(main_win);
		return 0;
}

void init_screen(void)
{
		attron(A_BOLD|A_UNDERLINE);
		mvprintw((LINES/2)-3, (COLS/2)-28, "OOView - Prints out OpenDocuments (odt) on your terminal");
		attroff(A_UNDERLINE);
		attron(A_BLINK);
		mvprintw((LINES/2)-2  , (COLS/2)-9,  "The VERY ALPHA");
		attroff(A_BLINK);
		mvprintw((LINES/2), (COLS/2)-10, "Press <x> to quit");
		attroff(A_BOLD);
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

int open_file(char *file){
if ((strstr(file,".ovd")!=NULL) || (strstr(file,".odt")!=NULL) )  /* check whether it's a ovd or odt*/
								{
										if (strstr(file,".ovd")!=NULL)			/* ovd*/
										{
												ovd_file = fopen(file,"r");
	
												if (ovd_file != NULL)
												{
														buffer = (struct fileinfo *)malloc(sizeof(struct fileinfo));
														main_win = subwin(stdscr,LINES-2,COLS,1,0);
														get_file_content(ovd_file, file, buffer);
														fclose(ovd_file);
														cur_char = buffer->content;
														print_site(buffer->cur_line, buffer->lines);
														print_file(buffer,cur_char);
														file_printed = true;
														file_type = 2;
												}
												else
												{
														print_status_bar("File does not exist!");
												}
										}
										else								/* else if odt */
										{
												buffer = (struct fileinfo *)malloc(sizeof(struct fileinfo));
												main_win= subwin(stdscr,LINES-2,COLS,1,0);
												open_odt(file, buffer);
/*												get_file_meta("/tmp/ooview/meta.xml",buffer);*/
												cur_char = buffer->content;
												print_site(buffer->cur_line, buffer->lines);
												print_file(buffer,cur_char);
												file_printed = true;
												file_type = 1;
										}
										
								}
								else
								{
										print_status_bar("Must be a OOView-file (ovd) or OpenDocument-file (odt)!");	
								}
}

int main (int argc, char **argv)
{

	logfilepath = getenv ("HOME");
	logfilepath = strcat (logfilepath, "/.ooview.log");
	
	load_config();
	
	
	initscr();
	raw();
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
  if (argc == 2) 
		open_file(argv[1]);
										
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
					if ((file_printed) && (buffer->cur_line > 1))
					{
							int backsteps = 0;
							int steps;
							char *tmp;
							
							if ((*--cur_char)==NEWLINE)
								backsteps++;

							tmp = cur_char - 1;

							if ((*--cur_char) == NEWLINE)
							{
									cur_char = tmp;
									print_status_bar("yeah");
							}
							else
							{
									do {
											cur_char--;
											backsteps++;
									} while (((*cur_char)!=NEWLINE) && (cur_char != buffer->content));
								
									if (backsteps > COLS)
									{
											int test;
											test = backsteps/COLS;
											steps = (backsteps%COLS);
											if (test>1)
													steps += COLS;
		
											mvwprintw(status_bar,0,0,"%d",steps);
											touchwin(status_bar);
											wrefresh(status_bar);
											cur_char += backsteps;
											cur_char -= steps;
									}
							}
							buffer->cur_line--;
							print_site(buffer->cur_line, buffer->lines);
							
							if (cur_char!=buffer->content)	
									print_file(buffer,++cur_char);
							else
									print_file(buffer,cur_char);
					}
					break;
			case KEY_DOWN:
					if ((file_printed) && (buffer->cur_line < buffer->lines))
					{
							int cols=0;
							
							
							while (((*cur_char)!=NEWLINE) && (cols < COLS-1))
							{		
									cols++;
									cur_char++;
							}
						
							
							buffer->cur_line++;
							print_site(buffer->cur_line, buffer->lines);
							print_file(buffer,++cur_char);
					}
					break;
			default:
					if (meta_win!=NULL)
					{
							delwin(meta_win);
							touchwin(stdscr);
							refresh();
					}
					
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
				char file[80];
				clear_status_bar();

				if (!strcmp(cmd,"Open"))
				{
						if (file_printed==false)
						{
								print_status_bar("Enter a file: ");
								curs_set(1);
								echo();
								wscanw(status_bar,"%s",file);			/* get filename from user */
								curs_set(0);
								noecho();
								open_file(file);

								
						}
						else
						{
								print_status_bar("Please close current file.");
						}
					
				}
				if (!strcmp(cmd,"Close"))
				{
						if (file_printed)
						{
								free(buffer);
								

								werase(main_win);
								init_screen();
								wrefresh(main_win);
								file_printed = false;
								system("rm -rf /tmp/ooview");
								

						}
						else
						{
								print_status_bar("No open file!");
						}
				}
				if (!strcmp(cmd,"Reload"))
				{
						if (file_printed)
						{
								if (strstr(file,".ovd")!=NULL)			/* ovd*/
										{
												ovd_file = fopen(file,"r");
	
												if (ovd_file != NULL)
												{
														free(buffer);
														buffer = (struct fileinfo *)malloc(sizeof(struct fileinfo));
														main_win = subwin(stdscr,LINES-2,COLS,1,0);
														get_file_content(ovd_file, file, buffer);
														fclose(ovd_file);
														cur_char = buffer->content;
														print_site(buffer->cur_line, buffer->lines);
														print_file(buffer,cur_char);
														file_printed = true;
												}
												else
												{
														print_status_bar("File does not exist!");
												}
										}
										else								/* else if odt */
										{
												free(buffer);
												buffer = (struct fileinfo *)malloc(sizeof(struct fileinfo));
												main_win= subwin(stdscr,LINES-2,COLS,1,0);
												open_odt(file,buffer);
												get_file_meta("/tmp/ooview/meta.xml",buffer);
												cur_char = buffer->content;
												print_site(buffer->cur_line, buffer->lines);
												print_file(buffer,cur_char);
												file_printed = true;
										}
								

						}
						else
						{
								print_status_bar("No open file!");
						}
				}

				if (!strcmp(cmd,"Document info"))
				{
						if (file_printed)
						{
								if (file_type==1)
								{
								
								meta_win = newwin(9,COLS-2,(LINES/2)-5,1);
								wbkgd(meta_win,COLOR_PAIR(4));
								

								mvwprintw(meta_win,1,1,"Genarator:  %s", buffer->generator);
								mvwprintw(meta_win,2,1,"Initial creator:\t%s", buffer->initial_creator);
								mvwprintw(meta_win,3,1,"Creation date:\t\t%s", buffer->creation_date);
								mvwprintw(meta_win,4,1,"Creator:\t\t%s", buffer->creator);
								mvwprintw(meta_win,5,1,"Date:\t\t\t%s", buffer->date);
								mvwprintw(meta_win,6,1,"Editing cycles:\t%s", buffer->editing_cycles);
								mvwprintw(meta_win,7,1,"Editing duration:\t%s", buffer->editing_duration);
								
								box(meta_win,0,0);
								wrefresh(meta_win);
								touchwin(meta_win);
								
								}
						}
						else
						{
								print_status_bar("No open file!");
						}
				}
				
				if (!strcmp(cmd,"OOView homepage"))
				{
						char *syscall;
						syscall=(char *)malloc(strlen(BROWSER)+strlen(HOMEPAGE_URL)+1);
						
						
						sprintf(syscall, "%s %s", BROWSER, HOMEPAGE_URL);
						system(syscall);
						free(syscall);
						if (file_printed)
							free(buffer);
						end_curses();
						return(0);
						
				}
						
				if (!strcmp(cmd,"Documentation"))
				{
						char *syscall;
						syscall=(char *)malloc(strlen(BROWSER)+strlen(HOMEPAGE_URL)+1);
						
						
						sprintf(syscall, "%s %s", BROWSER, HOMEPAGE_URL);
						system(syscall);
						free(syscall);
						if (file_printed)
							free(buffer);
						end_curses();
						return(0);
						
				}
				
				if (!strcmp(cmd, "About OOView"))
					print_status_bar("OOView. Visit Homepage for details");
				
				if (!strcmp(cmd, "Copying"))
					print_status_bar("OOView is under GPL/2 Visit Homepage for details");
				
				if (!strcmp(cmd, "Find")) {
					
					if (file_printed){
						
						print_status_bar("Enter string to find: ");
						char *findit;
						
						curs_set(1);
						echo();
						
						wscanw(status_bar,"%s",findit);
						
						/*
						
						how to find a string in a string?
						find it -> set cursor to position
						
						*/
						
						print_status_bar("Not implemented yet :(");
						curs_set(0);
						noecho();
					} else {
						print_status_bar("No open file!");
					}
				}
				
				if (!strcmp(cmd, "External Programs")) {
					char *prompt;
					
					/*getting printer command*/
					prompt = (char *)malloc((strlen("Enter Printing Command []: ")+strlen(PRINTER)));
					sprintf(prompt,"Enter Printing Command [%s]: ",PRINTER);
					print_status_bar(prompt);
					free(prompt);
					
					curs_set(1);
					echo();
					char *newcmd;
					wscanw(status_bar,"%s",newcmd);
					curs_set(0);
					noecho();
					print_status_bar(newcmd);
					/* / getting printer command*/	
					
				}
				
				if (!strcmp(cmd,"Exit"))
				{
						if (file_printed)
								free(buffer);
						end_curses();
						return 0;
				}
		}

	}
	if (file_printed)
			free(buffer);
	
	end_curses();
	return 0;
}

int write_odd (struct fileinfo *content) {
	/*writes the content of struct fileinfo to a temporary *.odd (=open document document) file*/
	const char *filename="/tmp/ooview/content.odd"; /*write content to this file, will be moved afterwards*/
	
	FILE *file;
	if ((file = fopen(filename,"wb")) != NULL)
	{
		fwrite(&content, sizeof(content), 1, file);
		fclose(file);
	}
	else
		return 7;
	return 0;
}

int olog (int errcode) {
	FILE *logfile;
	char *logstring;
	time_t time_now;
	
	
	logfile = fopen (logfilepath, "a");
	
	
	if (logfile != NULL) {
		/*log file format: 
			date  \n errcode - errdesc
			example:
			Wed May 31 10:36:50 2006
				Code: 13 - unknown error
		*/
		
		char *my_time;
		
		time_now = time(NULL);
		
		my_time = ctime(&time_now);
		fprintf(logfile,"%s   %i - %s\n", my_time, errcode, returnvalues[errcode]);
		
		fclose(logfile);
		return 0;
	}	else	{
		/*print error message to status bar (cant be written to logfile, eh)*/
		
		print_status_bar("Could not write to logfile. Check permissions!");
	}
	return 9;
	
}

int load_config(){
	FILE *configfile;
	
	if ((configfile = fopen(configpath, "r")) != NULL){
		int i=0;
		char buf;
		
		
		DEF_LANG=(char *)malloc(50);
		PRINTER=(char *)malloc(50);
		GFX_VIEWER=(char *)malloc(50);
		BROWSER=(char *)malloc(50);
		
		while ( buf != 10){
			buf=fgetc(configfile);
			if (buf != 10)
				DEF_LANG[i]=buf;
			
			i++;
		}
		i=0;
		buf=0;
		while ( buf != 10){
			buf=fgetc(configfile);
			if (buf != 10)
				PRINTER[i]=buf;
			
			i++;
		}
		i=0;
		buf=0;
				
		while ( buf != 10){
			buf=fgetc(configfile);
			if (buf != 10)
				GFX_VIEWER[i]=buf;
			
			i++;
		}
		i=0;
		buf=0;
		while ( buf != 10){
			buf=fgetc(configfile);
			if (buf != 10)
				BROWSER[i]=buf;
			
			i++;
		}
		
		
		fclose(configfile);
	} else {
		olog(14);
		print_status_bar("Could not open configfile");
		return 14;
	}
	return 0;
}
