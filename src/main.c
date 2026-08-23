#include <stdio.h>
#include <stdlib.h>
#include "argparse.h"
#include "imgstitch.h"

/* 程序入口：解析参数 → 打开子图 → 拼接 → 输出 */
int main(int argc, char *argv[])
{
    Imgs imgs;

    /* 1. 解析命令行参数（填充全局 options）并打印出来确认 */
    argparse(argc, argv); 
    print_args();

    /* 2. 用解析到的配置初始化 Imgs 结构体 */
    init_imgs(&imgs, &options);

    /* 3. 按命名约定逐个打开输入目录下的子图文件 */
    open_imgs(&imgs);
    
    /* 4. 执行拼接，得到输出大图句柄，并立即关闭保存到磁盘 */
    TIFF *stitched = img_stitch(&imgs);    
    TIFFClose(stitched);

    /* 5. 释放所有已打开的子图句柄与内存 */
    close_imgs(&imgs);
    return 0;
}