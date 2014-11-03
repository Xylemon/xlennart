#include "stdio.h"
#include "stdlib.h"

#include "types.h"
#include "util.h"

#include "Cable.h"
#include "Computer.h"
#include "Game.h"
#include "Network.h"
#include "OS.h"
#include "Spark.h"
#include "UI.h"

struct Cable {
	int c1, c2;		/* computers connected */
	int x1, y1, x2, y2;	/* endpoints of line representing cable */
	int x, y;		/* current location of spark */
	float fx, fy;		/* needed for line drawing */
	int delay;		/* how much time until spark leaves */
	int active;		/* is spark moving and from which end */
	int index;
};

#define SWAP(x, y) do {int _t; _t = x; x = y; y = _t;} while(0)

static void
reverse(Cable *cable) {
	SWAP(cable->c1, cable->c2);
	SWAP(cable->x1, cable->x2);
	SWAP(cable->y1, cable->y2);
}

void
Cable_setup(Cable **cablep) {
	Cable *cable;
	Computer *comp1, *comp2;
	int cwidth, cheight;

	cable = xalloc(sizeof *cable);

	cable->c1 = RAND(0, Network_num_computers() - 1);
	do {
		cable->c2 = RAND(0, Network_num_computers() - 1);
	} while (cable->c2 == cable->c1);
	cable->active = 0;
	cable->index = 0;
	cable->delay = SPARK_DELAY(Game_level());

	comp1 = Network_get_computer(cable->c1);
	comp2 = Network_get_computer(cable->c2);
	cwidth = Computer_width();
	cheight = Computer_height();
	cable->x1 = comp1->x + cwidth/3;
	cable->x2 = comp2->x + cwidth/3;
	cable->y1 = comp1->y + cheight/2;
	cable->y2 = comp2->y + cheight/2;

	*cablep = cable;
}


void
Cable_draw(Cable *cable) {
	UI_draw_line(cable->x1, cable->y1, cable->x2, cable->y2);
	if (cable->active) {
		int rx = cable->x - Spark_width()/2;
		int ry = cable->y - Spark_height()/2;
		Spark_draw(rx, ry, cable->index);
	}
}

void
Cable_update(Cable *cable) {
	Computer *comp1, *comp2;
	comp1 = Network_get_computer(cable->c1);
	comp2 = Network_get_computer(cable->c2);

	if (cable->active) {
		if ((comp1->os == OS_WINGDOWS) == (comp2->os == OS_WINGDOWS))
			cable->active = 0;
		else if (comp1->os == OS_WINGDOWS || comp2->os == OS_WINGDOWS) {
			int xdist, ydist;
			float sx, sy;

			if (comp2->os == OS_WINGDOWS)
				reverse(cable);

			xdist = cable->x2 - cable->x;
			ydist = cable->y2 - cable->y;

			sx = xdist >= 0 ? 1.0 : -1.0;
			sy = ydist >= 0 ? 1.0 : -1.0;
			xdist = abs(xdist);
			ydist = abs(ydist);
			if (xdist == 0 && ydist == 0) {
				if (!comp2->busy) {
					int counter;
					if (comp2->os == OS_OFF)
						counter = NETWORK_COUNTER_OFF;
					else
						counter = NETWORK_COUNTER_BASE;
					Network_inc_counter(counter, -1);
					Network_inc_counter(NETWORK_COUNTER_WIN,
							    1);
					comp2->os = OS_WINGDOWS;
				}
				cable->active = 0;
			}
			else if (MAX(xdist, ydist) < SPARK_SPEED) {
				cable->x = cable->x2;
				cable->y = cable->y2;
			}
			else {
				cable->fx+=(xdist*SPARK_SPEED*sx)/(xdist+ydist);
				cable->fy+=(ydist*SPARK_SPEED*sy)/(xdist+ydist);
				cable->x = (int)cable->fx;
				cable->y = (int)cable->fy;
			}
			cable->index = 1 - cable->index;
		}
	}
	else {
		if ((comp1->os == OS_WINGDOWS) == (comp2->os == OS_WINGDOWS))
			;
		else if (comp1->os == OS_WINGDOWS || comp2->os == OS_WINGDOWS) {
			cable->active = 1;
			cable->delay = SPARK_DELAY(Game_level());
			if (comp2->os == OS_WINGDOWS)
				reverse(cable);
			cable->x = cable->x1;
			cable->fx = cable->x1;
			cable->y = cable->y1;
			cable->fy = cable->y1;
		}
	}
}

int
Cable_onspark(Cable *cable, int locx, int locy) {
	if (!cable->active)
		return 0;
	return (abs(locx - cable->x) < Spark_width() &&
		abs(locy - cable->y) < Spark_height());
}

void
Cable_reset(Cable *cable) {
	cable->active = 0;
	cable->delay = SPARK_DELAY(Game_level());
}
