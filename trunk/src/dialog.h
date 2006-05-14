#ifndef __WIDGET_H
#define __WIDGET_H

typedef enum {
	WIDGET_INIT,		// Initialzie widget
	WIDGET_FOCUS,		// Draw widget in focused state
	WIDGET_UNFOCUS,		// Draw widget in unfocused state
	WIDGET_DRAW,		// Sent to widget to draw themselves
	WIDGET_KEY,			// Sent to widgets on key press
	WIDGET_HOTKEY,		// Sent to widget to catch preprocess key
	WIDGET_DESTROY,		// Sent to widget at destruction time
	WIDGET_CURSOR,		// Sent to widget to position the cursor
	WIDGET_IDLE,		// Sent to widget with options & W_WANT_IDLE
} widget_msg_t;

typedef enum {
	MSG_NOT_HANDLED,
	MSG_HANDLED
} cb_ret_t;

typedef enum {
	DLG_KEY,			// Key before sending to widget
	DLG_INIT,			// Initialize dialog
	DLG_END,			// Shut down dialog
	DLG_ACTION,			// State of check- and radioboxes has changed
	DLG_DRAW,			// Draw dialog on screen
	DLG_FOCUS,			// A widget has got focus
	DLG_UNFOCUS,		// A widget has been unfocused
	DLG_RESIZE,			// Window size has changed
	DLG_POST_KEY,		// The key has been handled
	DLG_IDLE,			// The idle state is active
	DLG_UNHANDLED_KEY,	// Key that no widget handled
	DLG_HOTKEY_HANDLED,	// A widget has got the hotkey
	DLG_VALIDATE		// Dialog is to be closed
} dlg_msg_t;

struct Dlg_head;
typedef cb_ret_t (*dlg_cb_fn)(struct Dlg_head *h, dlg_msg_t msg, int parm);

typedef struct Dlg_head {
	int flags;				// User flags
	const char *help_ctx;	// Name of the help entry
	const int *color;		// Color set
	char *title;			// Title of the dialog

	int ret_value;			// Result of run_dlg()

	/* Geometry */
	int x, y;				// Position relative to screen origin
	int cols, lines;		// Width and height of the window

	int running:1;			// The dialog is currently active
	int fullscreen:1;		// Parents dialogs don't net refresh

	int count;					// Number of widgets
	struct Widget *current;		// Currently active widget
	dlg_cb_fn callback;
	struct Dlg_head *parent;	// Parent dialog
} Dlg_head;

typedef struct Widget Widget;

typedef cb_ret_t (*callback_fn) (Widget *widget, widget_msg_t msg, int parm);

struct Widget {
	int x, y;
	int cols, lines;
	int options;
	int dlg_id;
	struct Widget *next;
	struct Widget *prev;
	callback_fn callback;		// The callback function
	struct Dlg_head *parent;
};

#endif
