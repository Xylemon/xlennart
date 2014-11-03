#ifndef HORDE_H
#define HORDE_H

#include "types.h"

/* Counters */
#define HORDE_COUNTER_OFF 0
#define HORDE_COUNTER_ON 1
#define HORDE_COUNTER_MAX 1

void Horde_setup(void);
void Horde_update(int iteration);
void Horde_draw(void);
Bill * Horde_get_bill(int index);
void Horde_move_bill(Bill *bill);
void Horde_remove_bill(Bill *bill);
void Horde_add_bill(Bill *bill);
Bill *Horde_clicked_stray(int x, int y);
int Horde_process_click(int x, int y);
void Horde_inc_counter(int counter, int val);
int Horde_get_counter(int counter);

#endif
