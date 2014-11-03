#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "util.h"

#include "Cable.h"
#include "Computer.h"
#include "Game.h"
#include "Network.h"
#include "OS.h"
#include "UI.h"

#define STD_MAX_COMPUTERS 20

static Computer *computers;
static int ncomputers;
static Cable **cables;
static int ncables;
static int counters[NETWORK_COUNTER_MAX + 1]; 	/* number in each state */

static int
on(int level) {
	int normal = MIN(8 + level, STD_MAX_COMPUTERS);
	return (int)(normal * Game_scale(2));
}

/* sets up network for each level */
void
Network_setup() {
	int i;	
	ncomputers = on(Game_level());
	if (computers != NULL)
		free(computers);
	if (cables != NULL) {
		for (i = 0; i < ncables; i++)
			if (cables[i] != NULL)
				free(cables[i]);
		free(cables);
	}
	computers = xalloc(ncomputers * sizeof(Computer));
	for (i = 0; i < ncomputers; i++)
		if (!Computer_setup(&computers[i], i)) {
			ncomputers = i - 1;
			break;
		}
	counters[NETWORK_COUNTER_OFF] = 0;
	counters[NETWORK_COUNTER_BASE] = ncomputers;
	counters[NETWORK_COUNTER_WIN] = 0;
	ncables = MIN(Game_level(), ncomputers/2);
	cables = xalloc(ncables * sizeof(Cable *));
	for (i = 0; i < ncables; i++)
		Cable_setup(&cables[i]);
}

/* redraws the computers at their location with the proper image */
void
Network_draw () {
	int i;
	for (i = 0; i < ncables; i++)
		Cable_draw(cables[i]);
	for (i = 0; i < ncomputers; i++)
		Computer_draw(&computers[i]);
}

void
Network_update () {
	int i;
	for (i = 0; i < ncables; i++)
		Cable_update(cables[i]);
}

void
Network_toasters () {
	int i;
	for (i = 0; i < ncomputers; i++) {
		computers[i].type = COMPUTER_TOASTER;
		computers[i].os = OS_OFF;
	}
	ncables = 0;
}

Computer *
Network_get_computer(int index) {
	return &computers[index];
}

int
Network_num_computers() {
	return ncomputers;
}

Cable *
Network_get_cable(int index) {
	return cables[index];
}

int
Network_num_cables() {
	return ncables;
}

void
Network_clear_stray(Bill *bill) {
	int i;
	for (i = 0; i < ncomputers; i++) {
		if (computers[i].stray == bill)
			computers[i].stray = NULL;
	}
}

void
Network_inc_counter(int counter, int val) {
	counters[counter] += val;
}

int
Network_get_counter(int counter) {
	return counters[counter];
}
