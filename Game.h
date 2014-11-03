#ifndef GAME_H
#define GAME_H

void Game_start(int newlevel);
void Game_quit(void);
void Game_warp_to_level(int lev);
void Game_add_high_score(const char *str);
void Game_button_press(int x, int y);
void Game_button_release(int x, int y);
void Game_update(void);

int Game_score(void);
int Game_level(void);
int Game_screensize(void);
double Game_scale(int dimensions);

#endif
