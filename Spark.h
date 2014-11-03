#ifndef SPARK_H
#define SPARK_H

#define SPARK_SPEED 4
#define SPARK_DELAY(level) (MAX(20 - (level), 0))

void
Spark_load_pix(void);

int 
Spark_width(void);

int
Spark_height(void);

void
Spark_draw(int x, int y, int index);


#endif
