#include <ctype.h>
#include <string.h>

#include "tty.h"
#include "menu.h"
#include "global.h"
#include "dialog.h"
#include "main.h"


Menu *create_menu (const char *name, menu_entry *entries, int count, const char *help_node)
{
    Menu *menu;
    const char *cp;

    menu = (Menu *) malloc (sizeof (*menu));
    menu->count = count;
    menu->max_entry_len = 20;
    menu->entries = entries;

    if (entries != (menu_entry*) NULL) {
	register menu_entry* mp;
	for (mp = entries; count--; mp++) {
	    if (mp->text[0] != '\0') {

	        cp = strchr (mp->text,'&');

		if (cp != NULL && *(cp+1) != '\0') {
		    mp->hot_key = tolower (*(cp+1));
		    menu->max_entry_len = max ((int) (strlen (mp->text) - 1),
			menu->max_entry_len);
		} else {
		    menu->max_entry_len = max ((int) strlen (mp->text),
			menu->max_entry_len);
		}
	    }
	}
    }

    menu->name = strdup (name);
    /* menu_scan_hotkey(menu); */
    menu->start_x = 0;
    menu->help_node = strdup (help_node);
    return menu;
}

