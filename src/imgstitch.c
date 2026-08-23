#include <stdio.h>
#include <stdlib.h>
#include <tiffio.h>
#include "argparse.h"
#include "imgstitch.h"

Canvas canvas = {0};

static int digit_nums(int a);

void open_imgs(Imgs *imgs, int rows, int cols, char const *imgdir)
{
    *imgs = (TIFF **)malloc(rows * cols * sizeof(TIFF *));
    if (*imgs == NULL)
    {
        fprintf(stderr, "Out of memory to allocate Imgs.\n");
        exit(EXIT_FAILURE);
    }

    char imgpath[256];
    int cnt = 1;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            snprintf(imgpath, 256, "%s/%0*d.tif", imgdir, digit_nums(rows * cols), cnt++);
            (*imgs)[i * cols + j] = TIFFOpen(imgpath, "r");
            if ((*imgs)[i * cols + j] == NULL)
            {
                fprintf(stderr, "Can't open tiff file.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void close_imgs(Imgs *imgs, int rows, int cols)
{
    for (int i = 0; i < rows * cols; i++)
    {
        if ((*imgs)[i] != NULL)
            TIFFClose((*imgs)[i]);
    }

    free(*imgs);
}

TIFF *get_imgs(Imgs *imgs, int cols, int a, int b)
{
    return *(*imgs + a * cols + b);
}

static int digit_nums(int a)
{
    int n = 0;
    
    // if a is 0, returns 1 directly
    if (a == 0) 
        return 1;

    while (a != 0)
    {
        a /= 10;
        n++;
    }
    return n;
}

void init_canvas(int rows, int cols)
{
    canvas.size = rows * cols;
    canvas.body = (char *)malloc(canvas.size * sizeof(char ));
    if (canvas.body == NULL)
    {
        fprintf(stderr, "Out of memory to allocate canvas.\n");
        exit(EXIT_FAILURE);
    }
}

void free_canvas()
{
    if (canvas.body)
        free(canvas.body);
}

TIFF *img_stitch(Imgs *imgs, int rows, int cols, int height, int width, char *outdir)
{
    char outpath[256];
    snprintf(outpath, 256, "%s/result.tif", outdir);
    TIFF *result = TIFFOpen(outpath, "w");

    TIFFSetField(result, TIFFTAG_IMAGEWIDTH, cols * width);
    TIFFSetField(result, TIFFTAG_IMAGELENGTH, cols * height);


    TIFFClose(result);
}