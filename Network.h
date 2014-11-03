#ifndef NETWORK_H
#define NETWORK_H

/* Counters */
#define NETWORK_COUNTER_OFF 0
#define NETWORK_COUNTER_BASE 1
#define NETWORK_COUNTER_WIN 2
#define NETWORK_COUNTER_MAX 2

void Network_setup(void);
void Network_draw(void);
void Network_update(void);
void Network_toasters(void);
Computer * Network_get_computer(int index);
int Network_num_computers(void);
Cable * Network_get_cable(int index);
int Network_num_cables(void);
void Network_clear_stray(Bill *bill);
void Network_inc_counter(int counter, int val);
int Network_get_counter(int counter);

#endif
