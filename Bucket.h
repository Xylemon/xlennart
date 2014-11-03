#ifndef BUCKET_H
#define BUCKET_H

void
Bucket_load_pix(void);

void
Bucket_draw(void);

int
Bucket_clicked(int x, int y);

void
Bucket_grab(int x, int y);

void
Bucket_release(int x, int y);

#endif
