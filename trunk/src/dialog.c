#include <string.h>
#include "global.h"
#include "tty.h"
#include "dialog.h"
#include "main.h"

/* Primitive way to check if the current dialog is our dialog */
Dlg_head *currentdlg = 0;
