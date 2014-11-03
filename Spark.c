#include "types.h"
#include "util.h"

#include "Spark.h"
#include "UI.h"

static Picture *pictures[2];

void
Spark_load_pix() {
	int i;
	for (i = 0; i < 2; i++)
		UI_load_picture_indexed("spark", i, 1, &pictures[i]);
}

int
Spark_width() {
	return UI_picture_width(pictures[0]);
}

int
Spark_height() {
	return UI_picture_height(pictures[0]);
}

void
Spark_draw(int x, int y, int index) {
	UI_draw(pictures[index], x, y);
}
