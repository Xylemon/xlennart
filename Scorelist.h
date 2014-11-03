#ifndef SCORELIST_H
#define SCORELIST_H

void Scorelist_read(void);
void Scorelist_write(void);
void Scorelist_recalc(const char *str, int level, int score);
void Scorelist_update(void);
int Scorelist_ishighscore(int val);

#endif
