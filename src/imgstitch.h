#ifndef IMGSTITCH_H
#define IMGSTITCH_H

#include <tiffio.h>
#include "argparse.h"

typedef struct convas
{
    int size;
    char *body;
}Convas;

typedef TIFF **Imgs;

extern Convas convas;

void open_imgs(Imgs *imgs, char const *imgdir);
void close_imgs(Imgs *imgs);
TIFF *get_imgs(Imgs *imgs, int a, int b);
void init_convas();
void free_convas();

#endif