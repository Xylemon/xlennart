#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "x11.h"
#include "util.h"

#include "Game.h"
#include "UI.h"

static int in_popup;

static const char *pictdir;

static Display *display;
static XtAppContext app;
static Drawable window, rootwindow;
static Colormap colormap;
static int depth;
static XColor white, black;
static Pixmap offscreen;
static XtIntervalId timer;
static GC stdgc, whitegc;
static int screensize;

static Widget toplevel, field;

/*
 * Callback functions
 */

static void
popdown(Widget w, XtPointer client_data, XtPointer call_data) {
	UNUSED(w);
	UNUSED(client_data);
	UNUSED(call_data);
	in_popup = 0;
}

void
x11_popup(Widget dialog) {
	Window temp;
	int tx, ty;
	XWindowAttributes tattr, pattr;
	int px, py;

	XtRealizeWidget(XtParent(dialog));
	XtSetMappedWhenManaged(XtParent(dialog), FALSE);
	XtManageChild(dialog);
	in_popup = 1;

	XTranslateCoordinates(display, XtWindow(toplevel), rootwindow,
			      0, 0, &tx, &ty, &temp);
	XGetWindowAttributes(display, XtWindow(toplevel), &tattr);
	XGetWindowAttributes(display, XtWindow(dialog), &pattr);
	px = tx + (tattr.width - pattr.width) / 2;
	py = ty + (tattr.height - pattr.height) / 2;
	XtVaSetValues(XtParent(dialog), XtNx, px, XtNy, py, NULL);

	XtAddCallback(XtParent(dialog), XtNpopdownCallback,
		(XtCallbackProc) popdown, NULL);
	XtPopup(XtParent(dialog), XtGrabExclusive);
	while (in_popup || XtAppPending(app))
		XtAppProcessEvent(app, XtIMXEvent);
}

/*
 * Event handlers
 */

static void
leave_window(Widget w, XtPointer client_data, XEvent *event, Boolean *b) {
	UNUSED(w);
	UNUSED(client_data);
	UNUSED(event);
	UNUSED(b);

	UI_pause_game();
}

static void
enter_window(Widget w, XtPointer client_data, XEvent *event, Boolean *b) {
	UNUSED(w);
	UNUSED(client_data);
	UNUSED(event);
	UNUSED(b);

	UI_resume_game();
}

static void
redraw_window(Widget w, XtPointer client_data, XEvent *event, Boolean *b) {
	UNUSED(w);
	UNUSED(client_data);
	UNUSED(event);
	UNUSED(b);

	UI_refresh();
}

static void
button_press(Widget w, XtPointer data, XEvent *event, Boolean *b) {
	XButtonEvent *buttonevent = (XButtonEvent *) event;

	UNUSED(w);
	UNUSED(data);
	UNUSED(b);

	Game_button_press(buttonevent->x, buttonevent->y);
}

static void
button_release(Widget w, XtPointer data, XEvent *event, Boolean *b) {
	XButtonEvent *buttonevent = (XButtonEvent *) event;

	UNUSED(w);
	UNUSED(data);
	UNUSED(b);

	Game_button_release(buttonevent->x, buttonevent->y);
}

static void
timer_tick(XtPointer client_data, XtIntervalId *timer_id) {
	UNUSED(client_data);
	UNUSED(timer_id);

	UI_restart_timer();
	Game_update();
}

/*
 * Cursor handling
 */

void
x11_set_cursor(MCursor *cursor) {
	XDefineCursor(display, window, cursor->cursor);
}

void
x11_load_cursor(const char *name, int masked, MCursor **cursorp) {
	MCursor *cursor;
	Pixmap bitmap, mask;
	int i, xh, yh;
	unsigned width, height;
	char file[255];

	cursor = xalloc(sizeof *cursor);

	sprintf(file, "%s/bitmaps/%s.xbm", pictdir, name);
	i = XReadBitmapFile(display, rootwindow, file,
			    &width, &height, &bitmap, &xh, &yh);
	if (i == BitmapOpenFailed)
		fatal("cannot open %s", file);
	if (masked == CURSOR_SEP_MASK) {
		sprintf(file, "%s/bitmaps/%s_mask.xbm", pictdir, name);
		i = XReadBitmapFile(display, rootwindow,
				    file, &width, &height, &mask, &xh, &yh);
		if (i == BitmapOpenFailed)
			fatal("cannot open %s", file);
	}
	else
		mask = bitmap;
	cursor->cursor = XCreatePixmapCursor(display, bitmap, mask,
					     &black, &white,
					     width/2, height/2);
	*cursorp = cursor;
}

/*
 * Pixmap handling
 */

void
x11_load_picture(const char *name, int trans, Picture **pictp) {
	Picture *pict;
	int i;
	char file[255];
	XpmColorSymbol symbol;
	Pixmap mask;
	XpmAttributes attr;
	unsigned long gcmask;
	XGCValues gcval;

	pict = xalloc(sizeof *pict);

	gcmask = GCForeground | GCBackground | GCGraphicsExposures;
	gcval.graphics_exposures = False;
	attr.valuemask = XpmCloseness | XpmReturnPixels | XpmColormap |
			 XpmDepth;
	attr.closeness = 65535;
	attr.colormap = colormap;
	attr.depth = depth;
	if (!trans) {
		symbol.name = NULL;
		symbol.value = "none";
		XtVaGetValues(field, XtNbackground, &symbol.pixel, NULL);
		attr.colorsymbols = &symbol;
		attr.numsymbols = 1;
		attr.valuemask |= XpmColorSymbols;
	}
	sprintf(file, "%s/pixmaps/%s.xpm", pictdir, name);
	i = XpmReadFileToPixmap(display, rootwindow, file, &pict->pix,
		&mask, &attr);
	if (i < 0)
		fatal("cannot open %s", file);
	pict->mask = mask;
	pict->gc = XCreateGC(display, offscreen, gcmask, &gcval);
	if (trans)
		XSetClipMask(display, pict->gc, mask);
	pict->width = attr.width;
	pict->height = attr.height;

	*pictp = pict;
}

void
x11_set_icon(Picture *icon) {
	XtVaSetValues(toplevel, XtNiconPixmap, icon->pix,
		      XtNiconMask, icon->mask, NULL);
}

int
x11_picture_width(Picture *pict) {
	return (pict->width);
}

int
x11_picture_height(Picture *pict) {
	return (pict->height);
}

/*
 * Graphics operations
 */

void
x11_graphics_init() {
	XGCValues gcval;
	unsigned long gcmask;
	gcmask = GCGraphicsExposures;
	gcval.graphics_exposures = False;
	stdgc = XCreateGC(display, window, gcmask, &gcval);
	XSetLineAttributes(display, stdgc, 2, LineSolid, CapRound, JoinMiter);
	XSetBackground(display, stdgc, white.pixel);
	XSetForeground(display, stdgc, black.pixel);
	whitegc = XCreateGC(display, window, gcmask, &gcval);
	XSetBackground(display, whitegc, white.pixel);
	XSetForeground(display, whitegc, white.pixel);
	offscreen = XCreatePixmap(display, rootwindow, screensize,
				  screensize, depth);
}

void
x11_clear_window() {
	XFillRectangle(display, offscreen, whitegc, 0, 0,
		       screensize, screensize);
}

void
x11_refresh_window() {
	XCopyArea(display, offscreen, window, stdgc, 0, 0,
		  screensize, screensize, 0, 0);
}

void
x11_draw_image(Picture *pict, int x, int y) {
	XSetClipOrigin(display, pict->gc, x, y);
	XCopyArea(display, pict->pix, offscreen, pict->gc, 0, 0,
		  pict->width, pict->height, x, y);
}

void
x11_draw_line(int x1, int y1, int x2, int y2) {
	XDrawLine(display, offscreen, stdgc, x1, y1, x2, y2);
}

void
x11_draw_string(const char *str, int x, int y) {
	XDrawString(display, offscreen, stdgc, x, y, str, strlen(str));
}

/*
 * Timer operations
 */

void
x11_start_timer(int ms) {
	timer = XtAppAddTimeOut(app, ms, timer_tick, NULL);
}

void
x11_stop_timer() {
	if (!timer)
		return;
	XtRemoveTimeOut(timer);
	timer = (XtIntervalId) 0;
}

int
x11_timer_active() {
	return (!!timer);
}

/*
 * Main Loop 
 */
void
x11_main_loop() {
	XtAppMainLoop(app);
}

/*
 * Initialization
 */
void
x11_initialize(int *argc, char **argv) {
	struct stat stats;

	timer = (XtIntervalId) 0;
	toplevel = XtAppInitialize(&app, "XBill", NULL, 0, argc, argv,
				   NULL, NULL, 0);
	display = XtDisplay(toplevel);

	if (stat(IMAGES, &stats) == 0)
		pictdir = IMAGES;
	else
		pictdir = ".";
}

void
x11_setup_resources() {
	XrmDatabase database;

	database = XrmGetDatabase(display);
	XrmPutStringResource(&database, "*background", "#c4c4c4");
	XrmPutStringResource(&database, "*foreground", "#000000");
	XrmSetDatabase(display, database);
}

void
x11_setup() {
	Screen *screen;
	XSizeHints h;
	Dimension winwidth, winheight;

	XtRealizeWidget(toplevel);
	screen = XtScreen(toplevel);
	depth = DefaultDepthOfScreen(screen);
	rootwindow = RootWindowOfScreen(screen);

	colormap = DefaultColormapOfScreen(screen);
	white.pixel = WhitePixelOfScreen(screen);
	XQueryColor(display, colormap, &white);
	black.pixel = BlackPixelOfScreen(screen);
	XQueryColor(display, colormap, &black);

	XtVaGetValues(toplevel, XtNwidth, &winwidth, XtNheight, &winheight,
		      NULL);
	h.width = h.base_width = h.min_width = h.max_width = winwidth;
	h.height = h.base_height = h.min_height = h.max_height = winheight;
	h.width_inc = h.height_inc = 0;
	h.flags = PSize|PMaxSize|PMinSize|PBaseSize|PResizeInc;
	XSetWMNormalHints(display, XtWindow(toplevel), &h);
}

void
x11_add_event_handlers(Widget w) {
	XtAddEventHandler(w, ButtonPressMask, FALSE, button_press, NULL);
	XtAddEventHandler(w, ButtonReleaseMask, FALSE, button_release, NULL);
	XtAddEventHandler(w, LeaveWindowMask, FALSE, leave_window, NULL);
	XtAddEventHandler(w, EnterWindowMask, FALSE, enter_window, NULL);
	XtAddEventHandler(w, ExposureMask, FALSE, redraw_window, NULL);
}

Widget
x11_toplevel() {
	return toplevel;
}

void
x11_set_drawingarea(Widget w, int size) {
	field = w;
	window = XtWindow(w);
	screensize = size;
}
