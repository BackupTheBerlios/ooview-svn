#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

int n_choices[4];

char *logfilepath;

char *BROWSER;
char *PRINTER;
char *GFX_VIEWER;
char *DEF_LANG;

struct fileinfo {
		char *content;
		int cur_line;
		int lines;
		char *generator;
		char *initial_creator;
		char *creation_date;
		char *creator;
		char *date;
		char *language;
		char *editing_cycles;
		char *editing_duration;
};

int write_odd(struct fileinfo *content);
int olog(int errcode);

void print_statsu_bar (char *text);
void print_site (int cur_line, int lines);
void open_odt (char *filename, struct fileinfo *buffer);
void get_file_content (FILE *ovd_file, char *filename, struct fileinfo *buffer);
int print_file (struct fileinfo *buffer, char *cur_char);
void init_screen(void);
void end_curses(void);
int open_file(char *filename);
