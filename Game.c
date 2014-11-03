#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "types.h"
#include "util.h"

#include "Bill.h"
#include "Bucket.h"
#include "Computer.h"
#include "Cable.h"
#include "Game.h"
#include "Horde.h"
#include "Network.h"
#include "OS.h"
#include "Scorelist.h"
#include "Spark.h"
#include "UI.h"

#define SCREENSIZE 400

/* Game states */
#define STATE_PLAYING 1
#define STATE_BETWEEN 2
#define STATE_END 3
#define STATE_WAITING 4

/* Score related constants */
#define SCORE_ENDLEVEL -1
#define SCORE_BILLPOINTS 5

static unsigned int state;
static int efficiency;
static int score, level, iteration;
static Picture *logo, *icon, *about;
static MCursor *defaultcursor, *downcursor;
static Bill *grabbed;
static const char *gui;
static int screensize = SCREENSIZE;

static void
setup_level(int newlevel) {
	level = newlevel;
	Horde_setup();
	grabbed = NULL;
	UI_set_cursor(defaultcursor);
	Network_setup();
	iteration = 0;
	efficiency = 0;
}

void
Game_start(int newlevel) {
	state = STATE_PLAYING;
	score = 0;
	UI_restart_timer();
	UI_set_pausebutton(1);
	setup_level(newlevel);
}

void
Game_quit() {
	Scorelist_write();
	exit(0);
}

static void
update_info(void) {
	char str[80];
	int on_screen = Horde_get_counter(HORDE_COUNTER_ON);
	int off_screen = Horde_get_counter(HORDE_COUNTER_OFF);
	int base = Network_get_counter(NETWORK_COUNTER_BASE);
	int off = Network_get_counter(NETWORK_COUNTER_OFF);
	int win = Network_get_counter(NETWORK_COUNTER_WIN);
	int units = Network_num_computers();
	sprintf(str, "Lenn:%d/%d  System:%d/%d/%d  Level:%d  Score:%d",
		on_screen, off_screen, base, off, win, level, score);
	UI_draw_str(str, 5, screensize - 5);
	efficiency += ((100 * base - 10 * win) / units);
}

void
Game_warp_to_level(int lev) {
	if (state == STATE_PLAYING) {
		if (lev <= level)
			return;
		setup_level(lev);
	}
	else {
		if (lev <= 0)
			return;
		Game_start(lev);
	}
}

void
Game_add_high_score(const char *str) {
	Scorelist_recalc(str, level, score);
}

void
Game_button_press(int x, int y) {
	int counter;

	if (state != STATE_PLAYING)
		return;
	UI_set_cursor(downcursor);

	if (Bucket_clicked(x, y)) {
		Bucket_grab(x, y);
		return;
	}

	grabbed = Horde_clicked_stray(x, y);
	if (grabbed != NULL) {
		OS_set_cursor(grabbed->cargo);
		return;
	}

	counter = Horde_process_click(x, y);
	score += (counter * counter * SCORE_BILLPOINTS);
}

void
Game_button_release(int x, int y) {
	int i;
	UI_set_cursor(defaultcursor);

	if (state != STATE_PLAYING)
		return;

	if (grabbed == NULL) {
		Bucket_release(x, y);
		return;
	}

	for (i = 0; i < Network_num_computers(); i++) {
		Computer *computer = Network_get_computer(i);

		if (Computer_on(computer, x, y) &&
		    Computer_compatible(computer, grabbed->cargo) &&
		    (computer->os == OS_WINGDOWS || computer->os == OS_OFF)) {
			int counter;

			Network_inc_counter(NETWORK_COUNTER_BASE, 1);
			if (computer->os == OS_WINGDOWS)
				counter = NETWORK_COUNTER_WIN;
			else
				counter = NETWORK_COUNTER_OFF;
			Network_inc_counter(counter, -1);
			computer->os = grabbed->cargo;
			Horde_remove_bill(grabbed);
			grabbed = NULL;
			return;
		}
	}
	Horde_add_bill(grabbed);
	grabbed = NULL;
}

static void
draw_logo(void) {
	UI_clear();
	UI_draw(logo,
		(screensize - UI_picture_width(logo)) / 2,
		(screensize - UI_picture_height(logo)) / 2);
}

void
Game_update() {
	char str[40];

	switch (state) {
	case STATE_PLAYING:
		UI_clear(); 
		Bucket_draw();
		Network_update();
		Network_draw();
		Horde_update(iteration);
		Horde_draw();
		update_info();
		if (Horde_get_counter(HORDE_COUNTER_ON) +
		    Horde_get_counter(HORDE_COUNTER_OFF) == 0) {
			score += (level * efficiency / iteration);
			state = STATE_BETWEEN;
		}
		if ((Network_get_counter(NETWORK_COUNTER_BASE) +
		     Network_get_counter(NETWORK_COUNTER_OFF)) <= 1)
			state = STATE_END;
		break;
	case STATE_END:
		UI_set_cursor(defaultcursor);
		UI_clear();
		Network_toasters();
		Network_draw();
		UI_refresh();
		UI_popup_dialog(DIALOG_ENDGAME);
		if (Scorelist_ishighscore(score)) {
			UI_popup_dialog(DIALOG_ENTERNAME);
			Scorelist_update();
		}
		UI_popup_dialog(DIALOG_HIGHSCORE);
		draw_logo();
		UI_kill_timer();
		UI_set_pausebutton(0);
		state = STATE_WAITING;
		break;
	case STATE_BETWEEN:
		UI_set_cursor(defaultcursor);
		sprintf(str, "After Level %d:\nScore: %d", level, score);
		UI_update_dialog(DIALOG_SCORE, str);
		UI_popup_dialog(DIALOG_SCORE);
		state = STATE_PLAYING;
		setup_level(++level);
		break;
	}
	UI_refresh();
	iteration++;
}

int
Game_score() {
	return score;
}

int
Game_level() {
	return level;
}

int
Game_screensize() {
	return screensize;
}

double
Game_scale(int dimensions) {
	double scale = (double)screensize / SCREENSIZE;
	double d = 1;
	for ( ; dimensions > 0; dimensions--)
		d *= scale;
	return (d);
}

/*
 * Note - don't use getopt, since it might reorder the args or do something
 * that the UI-specific argument parser doesn't like.
 */
static void
parse_args(int argc, char **argv) {
	char *s;
	char *endp;
	int i;

	for (i = 1; i < argc; i++) {
		if (strncasecmp(argv[i], "-l", 2) == 0) {
			if (strlen(argv[i]) == 2 && i == argc - 1)
				fatal("-l takes an argument");
			if (strlen(argv[i]) > 2)
				s = argv[i] + 2;
			else
				s = argv[++i];
			level = strtol(s, &endp, 10);
			if (*endp != '\0' || level <= 0)
				fatal("invalid level '%s'", s);
		} else if (strcmp(argv[i], "--gui") == 0) {
			if (i == argc - 1)
				fatal("--gui takes an argument");
			gui = argv[++i];
		} else if (strcmp(argv[i], "--size") == 0) {
			if (i == argc - 1)
				fatal("--size takes an argument");
			s = argv[++i];
			screensize = strtol(s, &endp, 10);
			if (*endp != '\0' || screensize <= 0)
				fatal("invalid screensize '%s'", s);
			if (screensize < SCREENSIZE)
				fatal("screensize must be larger than '%d'",
				       SCREENSIZE);
		} else if (strcmp(argv[1], "-v") == 0 ||
			   strcmp(argv[1], "--version") == 0)
		{
			printf ("XLennart version 1.0\n\n");
			exit(0);
		} else if (strcmp(argv[1], "-h") == 0 ||
			   strcmp(argv[1], "--help") == 0)
		{
			printf("XLennart version 1.0\n");
			printf("Options:\n");
			printf("-l <n>\tStart at level n.\n");
			printf("--gui <gui> \tUse a specific gui "
			       "(athena, motif, gtk)\n");
			printf("--size <size>\t\tUse a larger playing area.\n");
			printf("-v\t\tPrint version number and exit.\n");
			printf("-h\t\tPrint help and exit.\n");
			printf("All standard toolkit options are also ");
			printf("supported.\n\n");
			exit(0);
		}
	}
}

int
main(int argc, char **argv) {
	srand(time(NULL));
	parse_args(argc, argv);
	UI_initialize(gui, &argc, argv);
	UI_make_main_window(screensize);
	UI_graphics_init();
	UI_load_picture("logo", 0, &logo);
	UI_load_picture("icon", 0, &icon);
	UI_load_picture("about", 0, &about);
	draw_logo();
	UI_refresh();
	UI_make_dialogs(logo, icon, about);
	UI_set_icon(icon);

	Scorelist_read();
	Scorelist_update();

	UI_load_cursor("hand_up", CURSOR_SEP_MASK, &defaultcursor);
	UI_load_cursor("hand_down", CURSOR_SEP_MASK, &downcursor);
	UI_set_cursor(defaultcursor);

	Bill_load_pix();
	OS_load_pix();
	Computer_load_pix();
	Bucket_load_pix();
	Spark_load_pix();

	state = STATE_WAITING;
	if (level)
		Game_start(level);
	else
		UI_set_pausebutton(0);
	UI_main_loop();
	exit(0);
}
