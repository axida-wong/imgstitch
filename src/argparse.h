#ifndef ARGPARSE_H
#define ARGPARSE_H

/* 需要 GNU 扩展以启用 getopt_long 的 _GNU_SOURCE 行为 */
#define _GNU_SOURCE

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

/* 命令行参数选项结构体
 * rows    : 拼接网格的行数（r）
 * cols    : 拼接网格的列数（c）
 * height  : 单个子图的高度（像素，即 h）
 * width   : 单个子图的宽度（像素，即 w）
 * dir     : 存放输入子图的目录路径
 * out     : 输出图像保存的目录路径
 */
typedef struct opts
{
    int rows;
    int cols;
    int height;
    int width;
    char *dir;
    char *out;
}Opts;

/* 全局选项变量，由 argparse() 填充，供其他模块读取 */
extern Opts options;

/* 解析命令行参数并填充全局 options 变量 */
void argparse(int argc, char *argv[]);

/* 打印已解析出的各项参数（用于调试/确认） */
void print_args(void);

#endif