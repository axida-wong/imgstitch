#include <stdio.h>
#include <stdlib.h>
#include <tiffio.h>
#include <string.h>
#include "argparse.h"
#include "imgstitch.h"

// Canvas canvas = {0};

static int digit_nums(int a);

void init_imgs(Imgs *imgs, Opts *opts)
{
    imgs->opts = *opts; 
}

void open_imgs(Imgs *imgs)
{
    imgs->img_ptrs = (TIFF **)malloc(imgs->opts.rows * imgs->opts.cols * sizeof(TIFF *));

    if (imgs->img_ptrs == NULL)
    {
        fprintf(stderr, "Out of memory to allocate Imgs.\n");
        exit(EXIT_FAILURE);
    }

    char imgpath[256];
    int cnt = 1;

    for (int i = 0; i < imgs->opts.rows; i++)
    {
        for (int j = 0; j < imgs->opts.cols; j++)
        {
            snprintf(imgpath, 256, "%s/%0*d.tif", imgs->opts.dir, digit_nums(imgs->opts.rows * imgs->opts.cols), cnt++);
            imgs->img_ptrs[i * imgs->opts.cols + j] = TIFFOpen(imgpath, "r");
            if (imgs->img_ptrs[i * imgs->opts.cols + j] == NULL)
            {
                fprintf(stderr, "Can't open tiff file.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void close_imgs(Imgs *imgs)
{
    for (int i = 0; i < imgs->opts.rows * imgs->opts.cols; i++)
    {
        if (imgs->img_ptrs[i] != NULL)
            TIFFClose(imgs->img_ptrs[i]);
    }

    free(imgs->img_ptrs);
}

TIFF *get_imgs(Imgs *imgs, int i, int j)
{
    return imgs->img_ptrs[i * imgs->opts.cols + j];
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

// void init_canvas(int rows, int cols)
// {
//     canvas.size = rows * cols;
//     canvas.body = (char *)malloc(canvas.size * sizeof(char ));
//     if (canvas.body == NULL)
//     {
//         fprintf(stderr, "Out of memory to allocate canvas.\n");
//         exit(EXIT_FAILURE);
//     }
// }

// void free_canvas()
// {
//     if (canvas.body)
//         free(canvas.body);
// }

TIFF *img_stitch(Imgs *imgs)
{
    char outpath[256];
    snprintf(outpath, 256, "%s/result.tif", imgs->opts.out);
    TIFF *result = TIFFOpen(outpath, "w");


    // 假设从第一个子图 (*imgs)[0] 读取属性
    uint16_t samplesperpixel, bitspersample, photometric, planconfig;

    TIFFGetField(imgs->img_ptrs[0], TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel); // 如 3 (RGB) 或 1 (灰度)
    TIFFGetField(imgs->img_ptrs[0], TIFFTAG_BITSPERSAMPLE, &bitspersample);     // 如 8 或 16 bit
    TIFFGetField(imgs->img_ptrs[0], TIFFTAG_PHOTOMETRIC, &photometric);         // 如 PHOTOMETRIC_RGB
    TIFFGetField(imgs->img_ptrs[0], TIFFTAG_PLANARCONFIG, &planconfig);

    // 配置目标大图的 Header
    TIFFSetField(result, TIFFTAG_IMAGEWIDTH, imgs->opts.cols * imgs->opts.width);
    TIFFSetField(result, TIFFTAG_IMAGELENGTH, imgs->opts.rows * imgs->opts.height);
    TIFFSetField(result, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
    TIFFSetField(result, TIFFTAG_BITSPERSAMPLE, bitspersample);
    TIFFSetField(result, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(result, TIFFTAG_PLANARCONFIG, planconfig);

    int bytes_per_pixel = samplesperpixel * (bitspersample / 8);
    size_t tile_line_bytes = imgs->opts.width * bytes_per_pixel;           // 子图一行的字节数
    size_t full_line_bytes = imgs->opts.cols * imgs->opts.width * bytes_per_pixel;    // 拼接后大图一行的字节数

    // 分配用于存放“子图单行”和“大图单行”的内存 buffer
    tdata_t tile_buf = _TIFFmalloc(tile_line_bytes);
    tdata_t line_buf = _TIFFmalloc(full_line_bytes);

    // 1. 遍历网格的每一行子图 (r)
    for (int r = 0; r < imgs->opts.rows; r++)
    {
        // 2. 遍历当前子图内部的每一行像素 (y)
        for (int y = 0; y < imgs->opts.height; y++)
        {
            // 3. 遍历网格的每一列子图 (c)，拼接这一行横向的所有像素
            for (int c = 0; c < imgs->opts.cols; c++)
            {
                TIFF *tile = get_imgs(imgs, r, c);
                
                // 从当前子图读取第 y 行像素到 tile_buf
                if (TIFFReadScanline(tile, tile_buf, y, 0) < 0) {
                    fprintf(stderr, "Error reading scanline at tile (%d, %d), row %d\n", r, c, y);
                }
                
                // 将子图这一行复制到大图 buffer 的对应横向偏移位置
                memcpy((char *)line_buf + (c * tile_line_bytes), tile_buf, tile_line_bytes);
            }

            // 计算当前写入的是大图的第几行
            uint32_t out_row = r * imgs->opts.height + y;

            // 4. 将拼接好的一整行数据写入目标大图
            if (TIFFWriteScanline(result, line_buf, out_row, 0) < 0) {
                fprintf(stderr, "Error writing scanline %d\n", out_row);
            }
        }
    }

    // 释放临时内存
    _TIFFfree(tile_buf);
    _TIFFfree(line_buf);

    // TIFFClose(result);
    return result;
}