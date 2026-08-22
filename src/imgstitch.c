#include <stdio.h>
#include <stdlib.h>
#include <tiffio.h>
#include "argparse.h"
#include "imgstitch.h"

Convas convas = {0};

TIFF *get_imgs(Imgs *imgs, int a, int b)
{
    return *(*imgs + a * options.cols + b);
}

void open_imgs(Imgs *imgs, char const *imgdir)
{
    *imgs = (TIFF **)malloc(options.rows * options.cols * sizeof(TIFF *));
    if (*imgs == NULL)
    {
        fprintf(stderr, "Out of memory to allocate Imgs.\n");
        exit(EXIT_FAILURE);
    }

    char imgpath[256];
    int cnt = 1;

    for (int i = 0; i < options.rows; i++)
    {
        for (int j = 0; j < options.cols; j++)
        {
            snprintf(imgpath, 256, "%s/%d.tif", imgdir, cnt++);
            (*imgs)[i * options.cols + j] = TIFFOpen(imgpath, "r");
            if ((*imgs)[i * options.cols + j] == NULL)
            {
                fprintf(stderr, "Can't open tiff file.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void close_imgs(Imgs *imgs)
{
    for (int i = 0; i < options.rows * options.cols; i++)
    {
        if ((*imgs)[i] != NULL)
            TIFFClose((*imgs)[i]);
    }

    free(*imgs);
}

void init_convas()
{
    convas.size = options.rows * options.cols;
    convas.body = (char *)malloc(convas.size * sizeof(char ));
    if (convas.body)
    {
        fprintf(stderr, "Out of memory to allocate convas.\n");
        exit(EXIT_FAILURE);
    }
}

void free_convas()
{
    if (convas.body)
        free(convas.body);
}