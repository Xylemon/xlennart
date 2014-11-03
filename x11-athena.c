#include <stdio.h>
#include <stdlib.h>

#include "x11.h"
#include "x11-athena.h"

#include <X11/Xaw/XawInit.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/SmeLine.h>
#include <X11/Xaw/AsciiText.h>

#include "util.h"

#include "Game.h"
#include "UI.h"

static Widget base, menubar, field;
static Widget dialogs[DIALOG_MAX + 1];
static Widget pausebutton;

static void
x11_athena_popup_dialog(int index) {
	x11_popup(dialogs[index]);
}

static void
popup(Widget w, XtPointer client_data, XtPointer call_data) {
	UNUSED(w);
	UNUSED(call_data);
	x11_athena_popup_dialog((int)client_data);
}

static Widget
new_menu_item(Widget pshell, int dialog) {
	Widget menu_item = XtCreateManagedWidget(UI_menu_string(dialog),
						 smeBSBObjectClass,
						 pshell, NULL, 0);
	XtAddCallback(menu_item, XtNcallback, popup, (void *) dialog);
	return menu_item;
}

static Widget
CreateMenuBar(const char *name, Widget parent) {
	Widget menubar, pshell;
	Widget game_menu, info_menu;

	menubar = XtVaCreateManagedWidget(name, boxWidgetClass, parent,
					  XtNborderWidth, 0,
					  XtNorientation, XtEhorizontal, NULL);

	game_menu = XtCreateManagedWidget("Game", menuButtonWidgetClass,
					  menubar, NULL, 0);
	pshell = XtCreatePopupShell("menu", simpleMenuWidgetClass,
				    game_menu, NULL, 0);

	new_menu_item(pshell, DIALOG_NEWGAME);
	pausebutton = new_menu_item(pshell, DIALOG_PAUSEGAME);
	new_menu_item(pshell, DIALOG_WARPLEVEL);
	new_menu_item(pshell, DIALOG_HIGHSCORE);
	new_menu_item(pshell, DIALOG_QUITGAME);

	info_menu = XtCreateManagedWidget("Info", menuButtonWidgetClass,
					  menubar, NULL, 0);
	pshell = XtCreatePopupShell("menu", simpleMenuWidgetClass,
				    info_menu, NULL, 0);

	new_menu_item(pshell, DIALOG_STORY);
	new_menu_item(pshell, DIALOG_RULES);
	new_menu_item(pshell, DIALOG_ABOUT);

	return menubar;
}

static void
close_window (Widget w, XtPointer client_data, XtPointer call_data) {
	UNUSED(client_data);
	UNUSED(call_data);
	XtPopdown(XtParent(XtParent(w)));
}

static Widget
CreateRowCol(const char *name, Widget parent) {
	return XtCreateManagedWidget(name, boxWidgetClass, parent, NULL, 0);
}

static void
CreatePixmapBox(int index, Widget parent, Pixmap logo, Pixmap pixmap) {
	Widget base, pshell, button;
	const char *text = UI_dialog_string(index);

	pshell = XtCreatePopupShell(UI_menu_string(index),
				    transientShellWidgetClass, parent, NULL, 0);

	base = CreateRowCol("", pshell);

	XtVaCreateManagedWidget("", labelWidgetClass, base,
		XtNbitmap, logo, XtNborderWidth, 0, NULL);

	if (pixmap)
		XtVaCreateManagedWidget("", labelWidgetClass, base, 
					XtNbitmap, pixmap, XtNborderWidth,
					0, NULL);
	if (text != NULL)
		XtVaCreateManagedWidget("", labelWidgetClass, base, 
			XtNlabel, text, XtNborderWidth, 0, NULL);

	button = XtVaCreateManagedWidget("OK", commandWidgetClass, base, NULL);
	XtAddCallback(button, XtNcallback, close_window, NULL);
	dialogs[index] = base;
}

static void
CreateEnterText(int index, Widget parent, XtCallbackProc callback) {
	Widget base, pshell, button, textfield;
	pshell = XtCreatePopupShell(UI_menu_string(index),
				    transientShellWidgetClass, parent, NULL, 0);
	base = CreateRowCol("", pshell);
	XtVaCreateManagedWidget("", labelWidgetClass, base,
		XtNlabel, UI_dialog_string(index), XtNborderWidth, 0, NULL);

	textfield = XtVaCreateManagedWidget("", asciiTextWidgetClass, base,
		XtNeditType, XawtextEdit, XtNstring, "", XtNwidth, 200, NULL);

	button = XtVaCreateManagedWidget("OK", commandWidgetClass, base, NULL);
	XtAddCallback(button, XtNcallback, callback, textfield);
	XtAddCallback(button, XtNcallback, close_window, NULL);
	button = XtVaCreateManagedWidget("Cancel", commandWidgetClass, base,
		NULL);
	XtAddCallback(button, XtNcallback, close_window, NULL);
	dialogs[index] = base;
}

static void
CreateDialog(int index, Widget parent, int hascancel, Pixmap icon,
	     const char *buttonlabel, XtCallbackProc callback)
{
	Widget base, pshell, button;
	const char *text = UI_dialog_string(index);
	char ttext[16];

	pshell = XtCreatePopupShell(UI_menu_string(index),
				    transientShellWidgetClass, parent, NULL, 0);
	base = CreateRowCol("base", pshell);
	if (icon)
		XtVaCreateManagedWidget("", labelWidgetClass, base,
					XtNbitmap, icon,
					XtNborderWidth, 0, NULL);
	if (text != NULL && strlen(text) < sizeof(ttext)) {
		sprintf(ttext, "%-*s", (int)sizeof(ttext) - 1, text);
		text = ttext;
	}
	XtVaCreateManagedWidget("label", labelWidgetClass, base,
		XtNlabel, text, XtNborderWidth, 0, NULL);

	if (!buttonlabel)
		buttonlabel="OK";
	button = XtVaCreateManagedWidget(buttonlabel, commandWidgetClass,
					 base, NULL);
	if (callback)
		XtAddCallback(button, XtNcallback, callback, NULL);
	XtAddCallback(button, XtNcallback, close_window, NULL);

	if (hascancel) {
		button = XtVaCreateManagedWidget("Cancel", commandWidgetClass,
						 base, NULL);
		XtAddCallback(button, XtNcallback, close_window, NULL);
	}
	dialogs[index] = base;
}

static Widget
CreateDrawingArea(const char *name, Widget parent, int width, int height) {
	return XtVaCreateManagedWidget(name, coreWidgetClass, parent, XtNwidth,
		width, XtNheight, height, NULL);
}

static void
x11_athena_update_dialog(int index, const char *str) {
	WidgetList t;
	XtVaGetValues(dialogs[index], XtNchildren, &t, NULL);
	XtVaSetValues(t[0], XtNlabel, str, NULL);
}

static void
x11_athena_make_main_window(int size) {
	x11_setup_resources();
	base = XtVaCreateManagedWidget("base", boxWidgetClass, x11_toplevel(),
				       XtNhSpace, 0, XtNvSpace, 0, NULL);
	menubar = CreateMenuBar("menubar", base);
	field = CreateDrawingArea("field", base, size, size);
	x11_setup();
	x11_set_drawingarea(field, size);
	x11_add_event_handlers(field);
}

static void
new_game(Widget w, XtPointer client_data, XtPointer call_data) {
	UNUSED(w);
	UNUSED(client_data);
	UNUSED(call_data);
	Game_start(1);
}

static void
quit_game(Widget w, XtPointer client_data, XtPointer call_data) {
	UNUSED(w);
	UNUSED(client_data);
	UNUSED(call_data);
	Game_quit();
}

static void
warp_apply(Widget w, XtPointer client_data, XtPointer call_data) {
	char *str;
	char *endp;
	int newlevel;
	Widget text = (Widget) client_data;

	UNUSED(w);
	UNUSED(call_data);

	XtVaGetValues(text, XtNstring, &str, NULL);
	newlevel = strtol(str, &endp, 10);
	if (*endp != '\0')
		return;
	Game_warp_to_level(newlevel);
}

static void
enter_name(Widget w, XtPointer client_data, XtPointer call_data) {
	char *str;
	Widget text = (Widget) client_data;

	UNUSED(w);
	UNUSED(call_data);

	XtVaGetValues(text, XtNstring, &str, NULL);
	Game_add_high_score(str);
}

static void
x11_athena_create_dialogs(Picture *logo, Picture *icon, Picture *about) {
	CreateDialog(DIALOG_NEWGAME, base, 1, (Pixmap)NULL, NULL, new_game);
	CreateDialog(DIALOG_PAUSEGAME, base, 0, icon->pix, "Continue", NULL);
	CreateEnterText(DIALOG_WARPLEVEL, base, warp_apply);
	CreateDialog(DIALOG_HIGHSCORE, base, 0, (Pixmap)NULL, NULL, NULL);
	CreateDialog(DIALOG_QUITGAME, base, 1, (Pixmap)NULL, NULL, quit_game);

	CreatePixmapBox(DIALOG_STORY, base, logo->pix, (Pixmap)NULL);
	CreatePixmapBox(DIALOG_RULES, base, logo->pix, (Pixmap)NULL);
	CreatePixmapBox(DIALOG_ABOUT, base, logo->pix, about->pix);

	CreateDialog(DIALOG_SCORE, base, 0, (Pixmap)NULL, NULL, NULL);
	CreateDialog(DIALOG_ENDGAME, base, 0, (Pixmap)NULL, "Nuts!", NULL);
	CreateEnterText(DIALOG_ENTERNAME, base, enter_name);
}

static void
x11_athena_set_pausebutton(int active) {
	if (pausebutton != NULL)
		XtSetSensitive(pausebutton, active);
}

static struct UI_methods x11_athena_methods = {
	x11_set_cursor,
	x11_load_cursor,
	x11_load_picture,
	x11_set_icon,
	x11_picture_width,
	x11_picture_height,
	x11_graphics_init,
	x11_clear_window,
	x11_refresh_window,
	x11_draw_image,
	x11_draw_line,
	x11_draw_string,
	x11_start_timer,
	x11_stop_timer,
	x11_timer_active,
	x11_athena_popup_dialog,
	x11_main_loop,
	x11_initialize,
	x11_athena_make_main_window,
	x11_athena_create_dialogs,
	x11_athena_set_pausebutton,
	x11_athena_update_dialog,
};

void
x11_athena_setmethods(UI_methods **methodsp) {
	*methodsp = &x11_athena_methods;
}
