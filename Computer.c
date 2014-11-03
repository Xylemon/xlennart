#include <stdlib.h>

#include "types.h"
#include "util.h"

#include "Bill.h"
#include "Computer.h"
#include "Horde.h"
#include "Network.h"
#include "Game.h"
#include "OS.h"
#include "UI.h"

#define OS_OFFSET 4			/* offset of screen from 0,0 */
#define BORDER(size) (size / 10)	/* at least this far from a side */

#define MIN_PC 6		/* type >= MIN_PC means the computer is a PC */

static const char *cpuname[] = {"toaster", "maccpu", "nextcpu", "sgicpu",
				"suncpu", "os2cpu", "bsdcpu"};

#define NUM_SYS (sizeof(cpuname) / sizeof(cpuname[0]))

static Picture *pictures[NUM_SYS];		/* array of cpu pictures */
static int width, height;


static int
determineOS(Computer *computer) {
	if (computer->type < MIN_PC)
		return computer->type;
	else
		return OS_randpc();
}

int
Computer_setup(Computer *computer, int index) {
	int j, counter = 0, flag;
	int x, y;
	int screensize = Game_screensize();
	int border = BORDER(screensize);
	do {
		if (++counter > 4000)
			return 0;
		x = RAND(border, screensize - border - width);
		y = RAND(border, screensize - border - height);
		flag = 1;
		/* check for conflicting computer placement */
		for (j = 0; j < index && flag; j++) {
			Computer *c = Network_get_computer(j);
			int twidth = width - BILL_OFFSET_X + Bill_width();
			if (UI_intersect(x, y, twidth, height,
					 c->x, c->y, twidth, height))
				flag = 0;
		}
	} while (!flag);
	computer->x = x;
	computer->y = y;
	computer->type = RAND(1, NUM_SYS - 1);
	computer->os = determineOS(computer);
	computer->busy = 0;
	computer->stray = NULL;
	return 1;
}

int
Computer_on(Computer *computer, int locx, int locy) {
	return (abs(locx - computer->x) < width &&
		abs(locy - computer->y) < height);
}

int
Computer_compatible(Computer *computer, int system) {
	return (computer->type == system ||
		(computer->type >= MIN_PC && OS_ispc(system)));
}

void
Computer_draw(Computer *computer) {
	UI_draw(pictures[computer->type], computer->x, computer->y);
	if (computer->os != OS_OFF)
		OS_draw(computer->os,
			computer->x + OS_OFFSET, computer->y + OS_OFFSET);
}

void
Computer_load_pix() {
	unsigned int i;
	for (i = 0; i < NUM_SYS; i++)
		UI_load_picture(cpuname[i], 1, &pictures[i]);
	width = UI_picture_width(pictures[0]);
	height = UI_picture_height(pictures[0]);
}

int
Computer_width() {
	return width;
}

int
Computer_height() {
	return height;
}
