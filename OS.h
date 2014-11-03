#ifndef OS_H
#define OS_H

#define OS_WINGDOWS 0		/* OS 0 is the init */
#define OS_OFF -1		/* OS -1 means the computer is off */
#define OS_PC 6			/* OS >= PC means the OS is a PC OS */

void
OS_load_pix(void);

void
OS_draw(int index, int x, int y);

int
OS_width(void);

int
OS_height(void);

void
OS_set_cursor(int index);

int
OS_randpc(void);

int
OS_ispc(int index);


#endif
