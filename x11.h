#ifndef X11_WIDGETS_H
#define X11_WIDGETS_H

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xresource.h>
#include <X11/xpm.h>

#ifdef VMS
#include <signal.h>
#include <X11VMS/vmsutil.h>
#endif

#include "types.h"

struct MCursor {
	Pixmap cursor;
};

struct Picture {
	Dimension width, height;
	Pixmap pix;
	Pixmap mask;
	GC gc;
};

void x11_set_cursor(MCursor *cursor);
void x11_load_cursor(const char *name, int masked, MCursor **cursorp);
void x11_load_picture(const char *name, int trans, Picture **pictp);
void x11_set_icon(Picture *icon);
int x11_picture_width(Picture *pict);
int x11_picture_height(Picture *pict);
void x11_graphics_init(void);
void x11_clear_window(void);
void x11_refresh_window(void);
void x11_draw_image(Picture *pict, int x, int y);
void x11_draw_line(int x1, int y1, int x2, int y2);
void x11_draw_string(const char *str, int x, int y);
void x11_start_timer(int ms);
void x11_stop_timer(void);
int x11_timer_active(void);
void x11_main_loop(void);
void x11_initialize(int *argc, char **argv);
void x11_create_dialogs(Picture *logo, Picture *icon, Picture *about);

void x11_setup_resources(void);
void x11_setup(void);
void x11_add_event_handlers(Widget w);
void x11_popup (Widget dialog);
Widget x11_toplevel(void);
void x11_set_drawingarea(Widget w, int size);

#endif
