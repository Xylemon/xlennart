#ifndef BILL_H
#define BILL_H

/* Bill's states */
#define BILL_STATE_IN 1
#define BILL_STATE_AT 2
#define BILL_STATE_OUT 3
#define BILL_STATE_DYING 4
#define BILL_STATE_STRAY 5

/* Offsets from upper right of computer */
#define BILL_OFFSET_X 20
#define BILL_OFFSET_Y 3

struct Bill {
	int state;		/* what is it doing? */
	int index;		/* index of animation frame */
	Picture **cels;		/* array of animation frames */
	int x, y;		/* location */
	int target_x;		/* target x position */
	int target_y;		/* target y position */
	int target_c;		/* target computer */
	int cargo;		/* which OS carried */
	int x_offset;		/* accounts for width differences */
	int y_offset;		/* 'bounce' factor for OS carried */
	int sx, sy;		/* used for drawing extra OS during switch */
	Bill *prev, *next;
};

void Bill_enter(Bill **billp);
void Bill_draw(Bill *bill);
void Bill_update(Bill *bill);
void Bill_set_dying(Bill *bill);
int Bill_clicked(Bill *bill, int locx, int locy);
int Bill_clickedstray(Bill *bill, int locx, int locy);
void Bill_load_pix(void);
int Bill_width(void);
int Bill_height(void);
int Bill_get_state(Bill *bill);

#endif
