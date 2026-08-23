#ifndef IMGSTITCH_H
#define IMGSTITCH_H

#include <tiffio.h>
#include "argparse.h"

typedef struct canvas
{
    int size;
    char *body;
}Canvas;

typedef TIFF **Imgs;

extern Canvas canvas;

void open_imgs(Imgs *imgs, int rows, int cols, char const *imgdir);
void close_imgs(Imgs *imgs, int rows, int cols);
TIFF *get_imgs(Imgs *imgs, int cols, int a, int b);
void init_convas(int rows, int cols);
void free_convas();

TIFF *img_stitch(Imgs *imgs, int rows, int cols, int height, int width, char *outdir);

#endif