/* 命令行参数解析模块 */

#define _GNU_SOURCE

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

/* 前置声明：打印使用说明 */
static void print_help_menu(char *prog_name);

/* 命令行参数选项结构体（与 argparse.h 中的定义保持一致） */
typedef struct opts
{
    int rows;
    int cols;
    int height;
    int width;
    char *dir;
    char *out;
}Opts;

/* getopt_long 使用的长选项表：{"长选项名", 是否带参数, 短选项指针, 短选项字符} */
static const struct option longopts[] = 
{
    {"rows", 1, NULL, 'r'},
    {"cols", 1, NULL, 'c'},
    {"height", 1, NULL, 'h'},
    {"width", 1, NULL, 'w'},
    {"dir", 1, NULL, 'd'},
    {"out", 1, NULL, 'o'},
    {0,0,0,0}   /* 选项表结束标志 */
};

/* 全局选项实例，初始全 0 */
Opts options = {0};

/* 解析命令行参数，把结果写入全局 options */
void argparse(int argc, char *argv[])
{
    /* 输出目录默认取当前目录 "." */
    options.out = ".";

    /* 没有任何参数时打印帮助并退出 */
    if (argc < 2)
    {
        print_help_menu(*argv);
        exit(EXIT_FAILURE);
    }

    int opt;
    /* 依次读取命令行选项；格式串开头的 ':' 表示缺少参数时返回 ':' */
    while ((opt = getopt_long(argc, argv, ":r:c:h:w:d:o:", longopts, NULL)) != -1)
    {
        switch (opt)
        {
            case 'r':                 /* -r: 网格行数 */
                options.rows = atoi(optarg);
                break;
            case 'c':                 /* -c: 网格列数 */
                options.cols = atoi(optarg);
                break;
            case 'h':                 /* -h: 子图高度 */
                options.height = atoi(optarg);
                break;
            case 'w':                 /* -w: 子图宽度 */
                options.width = atoi(optarg);
                break;
            case 'd':                 /* -d: 输入子图目录 */
                options.dir = optarg;
                break;
            case 'o':                 /* -o: 输出目录 */
                options.out = optarg;
                break;
            case ':':                 /* 选项缺少必填参数 */
                fprintf(stderr, "option needs a value\n");
                exit(EXIT_FAILURE);
            case '?':                 /* 未识别的选项 */
                fprintf(stderr, "Unknown option %c\n", optopt);
                exit(EXIT_FAILURE);
            default:
                break;
        }
    }
}

/* 打印已解析出的各项参数，方便确认运行时的配置 */
void print_args(void)
{
    printf("rows:   %d\n"
           "cols:   %d\n"
           "height: %d\n"
           "width:  %d\n" 
           "dir:    %s\n"
           "out:    %s\n",
           options.rows, 
           options.cols,
           options.height,
           options.width,
           options.dir,
           options.out);
}

/* 打印命令行使用说明 */
static void print_help_menu(char *prog_name)
{
    printf("Usage : %s \\\n", prog_name);
    puts("\t-r rows_num \\");
    puts("\t-c cols_num \\");
    puts("\t-h height\\");
    puts("\t-w width\\");
    puts("\t-d input dir\\");
    puts("\t-o output dir, default directory is .");
}