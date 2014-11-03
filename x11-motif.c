#include <stdio.h>
#include <stdlib.h>

#include "x11.h"
#include "x11-motif.h"

#include <Xm/Xm.h>
#include <Xm/Label.h>
#include <Xm/RowColumn.h>
#include <Xm/DrawingA.h>
#include <Xm/PushB.h>
#include <Xm/CascadeB.h>
#include <Xm/MessageB.h>
#include <Xm/SelectioB.h>
#include <Xm/Text.h>

#include "util.h"

#include "Game.h"
#include "UI.h"

static Widget base, menubar, field;
static Widget dialogs[DIALOG_MAX + 1];
static Widget pausebutton;

static void
x11_motif_popup_dialog(int index) {
	x11_popup(dialogs[index]);
}

static void
popup(Widget w, XtPointer client_data, XtPointer call_data) {
	UNUSED(w);
	UNUSED(call_data);
	x11_motif_popup_dialog((int)client_data);
}

static Widget
new_menu_item(Widget pulldown, int dialog) {
	Widget menu_item = XtCreateManagedWidget(UI_menu_string(dialog),
						 xmPushButtonWidgetClass,
						 pulldown, NULL, 0);
	XtAddCallback(menu_item, XmNactivateCallback, popup, (void *) dialog);
	return menu_item;
}

static Widget
CreateMenuBar(const char *name, Widget parent) {
	Widget menubar, pulldown;
	Widget game_menu, info_menu;

	menubar = XmCreateMenuBar(parent, (char *)name, NULL, 0);
	XtManageChild(menubar);

	pulldown = XmCreatePulldownMenu(menubar, "menu", NULL, 0);
	game_menu = XtVaCreateManagedWidget("Game",
					    xmCascadeButtonWidgetClass,
					    menubar, XmNsubMenuId,
					    pulldown, NULL);

	new_menu_item(pulldown, DIALOG_NEWGAME);
	pausebutton = new_menu_item(pulldown, DIALOG_PAUSEGAME);
	new_menu_item(pulldown, DIALOG_WARPLEVEL);
	new_menu_item(pulldown, DIALOG_HIGHSCORE);
	new_menu_item(pulldown, DIALOG_QUITGAME);

	pulldown = XmCreatePulldownMenu(menubar, "menu", NULL, 0);
	info_menu = XtVaCreateManagedWidget("Info",
					    xmCascadeButtonWidgetClass,
					    menubar, XmNsubMenuId,
					    pulldown, NULL);

	new_menu_item(pulldown, DIALOG_STORY);
	new_menu_item(pulldown, DIALOG_RULES);
	new_menu_item(pulldown, DIALOG_ABOUT);

	return menubar;
}

static Widget
CreateRowCol(const char *name, Widget parent) {
	return XtVaCreateManagedWidget(name, xmRowColumnWidgetClass, parent,
		XmNorientation, XmVERTICAL, NULL);
}

static void
CreatePixmapBox(int index, Widget parent, Pixmap logo, Pixmap pixmap) {
	Arg wargs[2];
	XmString mstr;
	Widget dialog, base;
	const char *text = UI_dialog_string(index);

	mstr = XmStringCreateLtoR("", XmSTRING_DEFAULT_CHARSET);
	XtSetArg(wargs[0], XmNdialogStyle, XmDIALOG_APPLICATION_MODAL);
	XtSetArg(wargs[1], XmNmessageString, mstr);
	dialog = XmCreateMessageDialog(parent, (char *)UI_menu_string(index),
				       wargs, 2);

	base = CreateRowCol("", dialog);

	XtVaCreateManagedWidget("", xmLabelWidgetClass, base, XmNlabelType,
				XmPIXMAP, XmNlabelPixmap, logo, NULL);

	if (pixmap)
		XtVaCreateManagedWidget("", xmLabelWidgetClass, base,
					XmNlabelType, XmPIXMAP,
					XmNlabelPixmap, pixmap, NULL);

	if (text) {
		mstr = XmStringCreateLtoR((char *)text,
					  XmSTRING_DEFAULT_CHARSET);
		XtVaCreateManagedWidget(text, xmLabelWidgetClass, base,
			XmNlabelString, mstr, NULL);
		XmStringFree(mstr);
	}

	XtUnmanageChild(XmMessageBoxGetChild(dialog, XmDIALOG_CANCEL_BUTTON));
	XtUnmanageChild(XmMessageBoxGetChild(dialog, XmDIALOG_HELP_BUTTON));
	dialogs[index] = dialog;
}

static void
CreateEnterText(int index, Widget parent, XtCallbackProc callback) {
	Widget dialog;
	Arg wargs[2];
	XmString mstr = XmStringCreateLtoR((char *) UI_dialog_string(index),
					   XmSTRING_DEFAULT_CHARSET);
	XtSetArg(wargs[0], XmNdialogStyle, XmDIALOG_APPLICATION_MODAL);
	XtSetArg(wargs[1], XmNselectionLabelString, mstr);
	dialog = XmCreatePromptDialog(parent, (char *) UI_menu_string(index),
				      wargs, 2);
	XtUnmanageChild(XmSelectionBoxGetChild(dialog, XmDIALOG_HELP_BUTTON));
	XtUnmanageChild(XmSelectionBoxGetChild(dialog, XmDIALOG_APPLY_BUTTON));
	XtAddCallback(dialog, XmNokCallback, callback,
		      XmSelectionBoxGetChild(dialog, XmDIALOG_TEXT));
	dialogs[index] = dialog;
}

static void
CreateDialog(int index, Widget parent, int hascancel, Pixmap icon,
	     const char *buttonlabel, XtCallbackProc callback)
{
	Widget dialog;
	Arg wargs[2];
	XmString mstr = XmStringCreateLtoR((char *) UI_dialog_string(index),
					   XmSTRING_DEFAULT_CHARSET);
	XtSetArg(wargs[0], XmNdialogStyle, XmDIALOG_APPLICATION_MODAL);
	XtSetArg(wargs[1], XmNmessageString, mstr);
	dialog = XmCreateMessageDialog(parent, (char *) UI_menu_string(index),
				       wargs, 2);
	if (icon != (Pixmap) NULL)
		XtVaSetValues(dialog, XmNsymbolPixmap, icon, NULL);
	XmStringFree(mstr);
	if (!hascancel)
		XtUnmanageChild(XmMessageBoxGetChild(dialog,
						     XmDIALOG_CANCEL_BUTTON));
	XtUnmanageChild(XmMessageBoxGetChild(dialog, XmDIALOG_HELP_BUTTON));
	if (buttonlabel != NULL) {
		mstr = XmStringCreateLtoR((char *) buttonlabel,
					  XmSTRING_DEFAULT_CHARSET);
		XtVaSetValues(XmMessageBoxGetChild(dialog, XmDIALOG_OK_BUTTON),
			      XmNlabelString, mstr, NULL);
		XmStringFree(mstr);
	}
	if (callback != NULL)
		XtAddCallback(dialog, XmNokCallback, callback, NULL);
	dialogs[index] = dialog;
}

static Widget
CreateDrawingArea(const char *name, Widget parent, int width, int height) {
	return XtVaCreateManagedWidget(name, xmDrawingAreaWidgetClass, parent,
				       XtNwidth, width, XtNheight, height,
				       XmNresizePolicy, FALSE, NULL);
}

static void
x11_motif_update_dialog(int index, const char *str) {
	XmString mstr = XmStringCreateLtoR((char *) str,
					   XmSTRING_DEFAULT_CHARSET);
	XtVaSetValues(
		XmMessageBoxGetChild(dialogs[index], XmDIALOG_MESSAGE_LABEL),
		XmNlabelString, mstr, NULL);
	XmStringFree(mstr);
}

static void
x11_motif_make_main_window(int size) {
	x11_setup_resources();
	base = CreateRowCol("base", x11_toplevel());
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

	str = XmTextGetString(text);
	newlevel = strtol(str, &endp, 10);
	if (*endp != '\0')
		return;
	XtFree(str);
	Game_warp_to_level(newlevel);
}

static void
enter_name(Widget w, XtPointer client_data, XtPointer call_data) {
	char *str;
	Widget text = (Widget) client_data;

	UNUSED(w);
	UNUSED(call_data);

	str = XmTextGetString(text);
	Game_add_high_score(str);
	XtFree(str);
}

static void
x11_motif_create_dialogs(Picture *logo, Picture *icon, Picture *about) {
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
x11_motif_set_pausebutton(int active) {
	if (pausebutton != NULL)
		XtSetSensitive(pausebutton, active);
}

static struct UI_methods x11_motif_methods = {
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
	x11_motif_popup_dialog,
	x11_main_loop,
	x11_initialize,
	x11_motif_make_main_window,
	x11_motif_create_dialogs,
	x11_motif_set_pausebutton,
	x11_motif_update_dialog,
};

void
x11_motif_setmethods(UI_methods **methodsp) {
	*methodsp = &x11_motif_methods;
}
