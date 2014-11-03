#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strings.h"
#include "types.h"
#include "util.h"

#include "config.h"

#include "Game.h"
#include "UI.h"

#if USE_ATHENA
#include "x11-athena.h"
#endif

#if USE_MOTIF
#include "x11-motif.h"
#endif

#if USE_GTK
#include "gtk.h"
#endif

static int playing;
static UI_methods *methods;
static const char *dialog_strings[DIALOG_MAX + 1];
static const char *menu_strings[DIALOG_MAX + 1];

/*
 * Timer control routines
 */

void
UI_restart_timer() {
	methods->start_timer(200);
}

void
UI_kill_timer() {
	methods->stop_timer();
}

void
UI_pause_game() {
	if (methods->timer_active())
		playing = 1;
	UI_kill_timer();
}

void
UI_resume_game() {
	if (playing && !methods->timer_active())
		UI_restart_timer();
	playing = 0;
}

/*
 * Window routines
 */

typedef struct guimap {
	const char *name;
	void (*setmethods)(UI_methods **methodsp);
} guimap;

static guimap guis[] = {
#ifdef USE_GTK
	{"gtk", gtk_ui_setmethods},
#endif
#ifdef USE_MOTIF
	{"motif", x11_motif_setmethods},
#endif
#ifdef USE_ATHENA
	{"athena", x11_athena_setmethods},
#endif
	{NULL, NULL},
};

void
UI_initialize(const char *gui, int *argc, char **argv) {
	guimap *map;
	if (gui == NULL) {
		map = guis;
		if (map->name == NULL)
			fatal("no configured GUIs");
		map->setmethods(&methods);
	} else {
		for (map = guis; map->name != NULL; map++)
			if (strcasecmp(gui, map->name) == 0)
				break;
		if (map->name == NULL)
			fatal("GUI '%s' not found", gui);
		map->setmethods(&methods);
	}
	methods->initialize(argc, argv);
}

void
UI_make_main_window(int size) {
	menu_strings[DIALOG_NEWGAME] = newgame_menu_str;
	menu_strings[DIALOG_PAUSEGAME] = pause_menu_str;
	menu_strings[DIALOG_WARPLEVEL] = warp_menu_str;
	menu_strings[DIALOG_HIGHSCORE] = highscore_menu_str;
	menu_strings[DIALOG_QUITGAME] = quit_menu_str;
	menu_strings[DIALOG_STORY] = story_menu_str;
	menu_strings[DIALOG_RULES] = rules_menu_str;
	menu_strings[DIALOG_ABOUT] = about_menu_str;
	menu_strings[DIALOG_SCORE] = score_menu_str;
	menu_strings[DIALOG_ENDGAME] = endgame_menu_str;
	menu_strings[DIALOG_ENTERNAME] = entername_menu_str;
	methods->make_main_window(size);
}

void
UI_graphics_init() {
	methods->graphics_init();
}

void
UI_make_dialogs(Picture *logo, Picture *icon, Picture *about) {
	dialog_strings[DIALOG_NEWGAME] = newgame_dialog_str;
	dialog_strings[DIALOG_PAUSEGAME] = pause_dialog_str;
	dialog_strings[DIALOG_WARPLEVEL] = warp_dialog_str;
	dialog_strings[DIALOG_HIGHSCORE] = NULL;
	dialog_strings[DIALOG_QUITGAME] = quit_dialog_str;
	dialog_strings[DIALOG_STORY] = story_dialog_str;
	dialog_strings[DIALOG_RULES] = rules_dialog_str;
	dialog_strings[DIALOG_ABOUT] = NULL;
	dialog_strings[DIALOG_SCORE] = NULL;
	dialog_strings[DIALOG_ENDGAME] = endgame_dialog_str;
	dialog_strings[DIALOG_ENTERNAME] = entername_dialog_str;
	methods->create_dialogs(logo, icon, about);
}

void
UI_popup_dialog(int dialog) {
	methods->popup_dialog(dialog);
}

/*
 * Graphics routines
 */

void
UI_set_cursor(MCursor *cursor) {
	methods->set_cursor(cursor);
}

void
UI_set_icon(Picture *icon) {
	methods->set_icon(icon);
}

void
UI_clear() {
	methods->clear_window();
}

void
UI_refresh() {
	methods->refresh_window();
}

void
UI_draw(Picture *pict, int x, int y) {
	methods->draw_image(pict, x, y);
}

void
UI_draw_line(int x1, int y1, int x2, int y2) {
	methods->draw_line(x1, y1, x2, y2);
}

void
UI_draw_str(const char *str, int x, int y) {
	methods->draw_string(str, x, y);
}

/*
 * Other routines
 */

void
UI_set_pausebutton (int action) {
	methods->set_pausebutton(action);
}

void
UI_main_loop() {
	methods->main_loop();
}

void
UI_load_picture_indexed(const char *name, int index, int trans, Picture **pictp)
{
	char *newname;
	if (index > 99)
		fatal("image index too large");
	newname = xalloc(strlen(name) + 4);
	sprintf(newname, "%s_%d", name, index);
	UI_load_picture(newname, trans, pictp);
	free(newname);
}

void
UI_load_picture(const char *name, int trans, Picture **pictp) {
	methods->load_picture(name, trans, pictp);
}

int
UI_picture_width(Picture *pict) {
	return methods->picture_width(pict);
}

int
UI_picture_height(Picture *pict) {
	return methods->picture_height(pict);
}

void
UI_load_cursor(const char *name, int masked, MCursor **cursorp) {
	methods->load_cursor(name, masked, cursorp);
}

int
UI_intersect(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
	return ((abs(x2 - x1 + (w2 - w1) / 2) < (w1 + w2) / 2) &&
		(abs(y2 - y1 + (h2 - h1) / 2) < (h1 + h2) / 2));
}

void
UI_update_dialog(int index, const char *str) {
	methods->update_dialog(index, str);
}

const char *
UI_dialog_string(int index) {
	return dialog_strings[index];
}

const char *
UI_menu_string(int index) {
	return menu_strings[index];
}
