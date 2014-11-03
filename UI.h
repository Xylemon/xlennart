#ifndef X11_UI_H
#define X11_UI_H

#include "types.h"

#define CURSOR_SEP_MASK 0
#define CURSOR_OWN_MASK 1

#define DIALOG_NEWGAME 0
#define DIALOG_PAUSEGAME 1
#define DIALOG_WARPLEVEL 2
#define DIALOG_HIGHSCORE 3
#define DIALOG_QUITGAME 4
#define DIALOG_STORY 5
#define DIALOG_RULES 6
#define DIALOG_ABOUT 7
#define DIALOG_SCORE 8
#define DIALOG_ENDGAME 9
#define DIALOG_ENTERNAME 10
#define DIALOG_MAX 10

void UI_restart_timer(void);
void UI_kill_timer(void);

void UI_pause_game(void);
void UI_resume_game(void);

void UI_initialize(const char *gui, int *argc, char **argv);
void UI_make_main_window(int size);
void UI_graphics_init(void);
void UI_make_dialogs(Picture *logo, Picture *icon, Picture *about);

void UI_popup_dialog(int index);
void UI_set_cursor(MCursor *cursor);
void UI_set_icon(Picture *icon);
void UI_clear(void);
void UI_refresh(void);
void UI_draw(Picture *picture, int x, int y);
void UI_draw_line(int x1, int y1, int x2, int y2);
void UI_draw_str(const char *str, int x, int y);

void UI_set_pausebutton(int action);
void UI_main_loop(void);

void UI_update_dialog(int index, const char *str);

void UI_load_picture(const char *name, int trans, Picture **pictp);
void UI_load_picture_indexed(const char *name, int index, int trans,
			     Picture **pictp);
int UI_picture_width(Picture *pict);
int UI_picture_height(Picture *pict);

void UI_load_cursor(const char *name, int masked, MCursor **cursorp);

int
UI_intersect(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

const char *UI_dialog_string(int index);
const char *UI_menu_string(int index);

typedef struct UI_methods {
	void	(*set_cursor)(MCursor *cursor);
	void	(*load_cursor)(const char *name, int masked, MCursor **cursorp);
	void	(*load_picture)(const char *name, int trans, Picture **pictp);
	void	(*set_icon)(Picture *pict);
	int	(*picture_width)(Picture *pict);
	int	(*picture_height)(Picture *pict);
	void	(*graphics_init)(void);
	void	(*clear_window)(void);
	void	(*refresh_window)(void);
	void	(*draw_image)(Picture *pict, int x, int y);
	void	(*draw_line)(int x1, int y1, int x2, int y2);
	void	(*draw_string)(const char *str, int x, int y);
	void	(*start_timer)(int ms);
	void	(*stop_timer)(void);
	int	(*timer_active)(void);
	void	(*popup_dialog)(int index);
	void	(*main_loop)(void);
	void	(*initialize)(int *argc, char **argv);
	void	(*make_main_window)(int size);
	void	(*create_dialogs)(Picture *logo, Picture *icon, Picture *about);
	void	(*set_pausebutton)(int active);
	void	(*update_dialog)(int index, const char *str);
} UI_methods;

#endif
