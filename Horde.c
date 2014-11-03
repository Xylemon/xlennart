#include <stdlib.h>

#include "types.h"
#include "util.h"

#include "Bill.h"
#include "Computer.h"
#include "Game.h"
#include "Horde.h"
#include "Network.h"
#include "UI.h"

static Bill *alive, *strays;
static int counters[HORDE_COUNTER_MAX + 1];

#define MAX_BILLS 100		/* max Bills per level */

#define UNLINK(bill, list)						\
	do {							\
		if ((bill)->next != NULL)			\
			(bill)->next->prev = (bill)->prev;	\
		if ((bill)->prev != NULL)			\
			(bill)->prev->next = (bill)->next;	\
		else if ((bill) == list)			\
			(list) = (bill)->next;			\
		(bill)->prev = NULL;				\
		(bill)->next = NULL;				\
	} while (0)

#define PREPEND(bill, list)					\
	do {							\
		(bill)->next = (list);				\
		if ((list) != NULL)				\
			(list)->prev = (bill);			\
		(list) = (bill);				\
	} while (0)

static int
on(unsigned int lev) {
	int perlevel = (int)((8 + 3 * lev) * Game_scale(2));
	return MIN(perlevel, MAX_BILLS);
}

static int
max_at_once(unsigned int lev) {
	return MIN(2 + lev / 4, 12);
}

static int
between(unsigned int lev) {
	return MAX(14 - lev / 3, 10);
}

/*  Launches Bills whenever called  */
static void
launch(int max) {
	Bill *bill;
	int n;
	int off_screen = counters[HORDE_COUNTER_OFF];

	if (max == 0 || off_screen == 0)
		return;
	n = RAND(1, MIN(max, off_screen));
	for (; n > 0; n--) {
		Bill_enter(&bill);
		PREPEND(bill, alive);
	}
}

void
Horde_setup() {
	Bill *bill;
	while (alive != NULL) {
		bill = alive;
		UNLINK(bill, alive);
		free(bill);
	}
	while (strays != NULL) {
		bill = strays;
		UNLINK(bill, strays);
		free(bill);
	}
	counters[HORDE_COUNTER_OFF] = on(Game_level());
	counters[HORDE_COUNTER_ON] = 0;
}

void
Horde_update(int iteration) {
	Bill *bill, *next;
	int level = Game_level();
	if (iteration % between(level) == 0)
		launch(max_at_once(level));
	for (bill = alive; bill != NULL; bill = next) {
		next = bill->next;
		Bill_update(bill);
	}
}

void
Horde_draw() {
	Bill *bill;

	for (bill = strays; bill != NULL; bill = bill->next)
		Bill_draw(bill);
	for (bill = alive; bill != NULL; bill = bill->next)
		Bill_draw(bill);
}

void
Horde_move_bill(Bill *bill) {
	UNLINK(bill, alive);
	PREPEND(bill, strays);
}

void
Horde_remove_bill(Bill *bill) {
	if (bill->state == BILL_STATE_STRAY)
		UNLINK(bill, strays);
	else
		UNLINK(bill, alive);
	Network_clear_stray(bill);
	free(bill);
}

void
Horde_add_bill(Bill *bill) {
	if (bill->state == BILL_STATE_STRAY)
		PREPEND(bill, strays);
	else
		PREPEND(bill, alive);
}

Bill *
Horde_clicked_stray(int x, int y) {
	Bill *bill;

	for (bill = strays; bill != NULL; bill = bill->next) {
		if (!Bill_clickedstray(bill, x, y))
			continue;
		UNLINK(bill, strays);
		return bill;
	}
	return NULL;
}

int
Horde_process_click(int x, int y) {
	Bill *bill;
	int counter = 0;

	for (bill = alive; bill != NULL; bill = bill->next) {
		if (bill->state == BILL_STATE_DYING ||
		    !Bill_clicked(bill, x, y))
			continue;
		if (bill->state == BILL_STATE_AT) {
			Computer *comp;
			comp = Network_get_computer(bill->target_c);
			comp->busy = 0;
			comp->stray = bill;
		}
		Bill_set_dying(bill);
       		counter++;
	}
	return counter;
}

void
Horde_inc_counter(int counter, int val) {
	counters[counter] += val;
}

int
Horde_get_counter(int counter) {
	return counters[counter];
}
