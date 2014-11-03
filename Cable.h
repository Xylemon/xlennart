#ifndef CABLE_H
#define CABLE_H

#include "types.h"

void Cable_setup(Cable **cablep);
void Cable_draw(Cable *cable);
void Cable_update(Cable *cable);
int Cable_onspark(Cable *cable, int locx, int locy);
void Cable_reset(Cable *cable);

#endif
