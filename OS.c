#include "types.h"
#include "util.h"

#include "OS.h"
#include "UI.h"

#define MIN_PC 6		/* OS >= MIN_PC means the OS is a PC OS */

static const char *osname[] = { "initfail", "arch", "bsd", "centos", "debian",
			         "gentoo", "mandriva", "openbsd", "slackware",
				 "suse", "ubuntu"};
#define NUM_OS (sizeof(osname) / sizeof(osname[0]))

static Picture *os[NUM_OS];		/* array of OS pictures*/
static MCursor *cursor[NUM_OS];		/* array of OS cursors (drag/drop) */


void
OS_load_pix() {
	unsigned int i;
	for (i = 0; i < NUM_OS; i++) {
		UI_load_picture(osname[i], 1, &os[i]);
		if (i != 0)
			UI_load_cursor(osname[i], CURSOR_OWN_MASK, &cursor[i]);
	}
}

void
OS_draw(int index, int x, int y) {
	UI_draw(os[index], x, y);
}

int
OS_width() {
	return UI_picture_width(os[0]);
}

int
OS_height() {
	return UI_picture_height(os[0]);
}

void
OS_set_cursor(int index) {
	UI_set_cursor(cursor[index]);
}

int
OS_randpc() {
	return (RAND(MIN_PC, NUM_OS - 1));
}

int
OS_ispc(int index) {
	return (index >= MIN_PC);
}
