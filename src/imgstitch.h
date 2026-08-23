#ifndef IMGSTITCH_H
#define IMGSTITCH_H

#include <tiffio.h>
#include "argparse.h"

// typedef struct canvas
// {
//     char *body;
//     size_t length;
//     size_t memb_size;
//     size_t total_size;
// } Canvas;

typedef struct imgs
{
    TIFF **img_ptrs;
    Opts opts;
} Imgs;

// extern Canvas canvas;

void init_imgs(Imgs *imgs, Opts *opts);
void open_imgs(Imgs *imgs);
void close_imgs(Imgs *imgs);
TIFF *get_imgs(Imgs *imgs, int i, int j);
TIFF *img_stitch(Imgs *imgs);

#endif