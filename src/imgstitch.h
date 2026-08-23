#ifndef IMGSTITCH_H
#define IMGSTITCH_H

#include <tiffio.h>
#include "argparse.h"

/* ---- 下列 Canvas 相关代码已废弃，仅作为预留方案保留，暂不使用 ---- */
// typedef struct canvas
// {
//     char *body;         /* 画布数据缓冲 */
//     size_t length;      /* 已用字节数 */
//     size_t memb_size;   /* 每次扩容的增量 */
//     size_t total_size;  /* 分配的缓冲区总大小 */
// } Canvas;

/* 输入子图集合
 * img_ptrs : 指向所有子图 TIFF 句柄的数组（按行优先存放，长度 rows*cols）
 * opts     : 命令行解析得到的配置参数
 */
typedef struct imgs
{
    TIFF **img_ptrs;
    Opts opts;
} Imgs;

// extern Canvas canvas;

/* 初始化 Imgs：把命令行参数拷贝进结构体 */
void init_imgs(Imgs *imgs, Opts *opts);
/* 打开目录下的所有子图文件并缓存句柄 */
void open_imgs(Imgs *imgs);
/* 关闭所有子图句柄并释放内存 */
void close_imgs(Imgs *imgs);
/* 获取第 i 行、第 j 列的子图句柄 */
TIFF *get_imgs(Imgs *imgs, int i, int j);
/* 拼接所有子图为一张大图，返回输出 TIFF 句柄 */
TIFF *img_stitch(Imgs *imgs);

#endif