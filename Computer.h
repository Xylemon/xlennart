#ifndef COMPUTER_H
#define COMPUTER_H

struct Computer {
	int type;		/* CPU type */
	int os;			/* current OS */
	int x, y;		/* location */
	int busy;		/* is the computer being used? */
	Bill *stray;
};


int Computer_setup(Computer *computer, int i);
void Computer_draw(Computer *computer);
int Computer_on (Computer *computer, int locx, int locy);
int Computer_compatible(Computer *computer, int system);
void Computer_load_pix(void);
int Computer_width(void);
int Computer_height(void);

#define COMPUTER_TOASTER 0	/* computer 0 is a toaster */

#endif
