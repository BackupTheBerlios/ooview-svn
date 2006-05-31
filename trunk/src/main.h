
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

int n_choices[4];

char *logfilepath;
char *configpath="/etc/ooview/config";

char *BROWSER;
char *PRINTER;
char *GFX_VIEWER;
char *DEF_LANG;

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

struct fileinfo
{
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
FILE *open_odt (char *filename);
void get_file_content (FILE *ovd_file, char *filename, struct fileinfo *buffer);
void get_file_meta (FILE *odt_file, struct fileinfo *buffer);
int print_file (struct fileinfo *buffer, char *cur_char);
void init_screen(void);
void end_curses(void);
