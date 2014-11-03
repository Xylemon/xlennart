#include <util.h>

#include "Bucket.h"
#include "Cable.h"
#include "Game.h"
#include "Network.h"
#include "UI.h"

static Picture *picture;
static MCursor *cursor;
static int grabbed;

void
Bucket_load_pix() {
	UI_load_picture("bucket", 1, &picture);
	UI_load_cursor("bucket", CURSOR_OWN_MASK, &cursor);
}

int
Bucket_clicked(int x, int y) {
	return (x > 0 && x < UI_picture_width(picture) &&
		y > 0 && y < UI_picture_height(picture));
}

void
Bucket_draw() {
	if (!grabbed)
		UI_draw(picture, 0, 0);
}

void
Bucket_grab(int x, int y) {
	UNUSED(x);
	UNUSED(y);

	UI_set_cursor(cursor);
	grabbed = 1;
}

void
Bucket_release(int x, int y) {
	int i;
	for (i = 0; i < Network_num_cables(); i++) {
		Cable *cable = Network_get_cable(i);
		if (Cable_onspark(cable, x, y))
			Cable_reset(cable);
	}
	grabbed = 0;
}
