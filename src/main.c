#include <stdio.h>
#include <stdlib.h>
#include "argparse.h"
#include "imgstitch.h"

int main(int argc, char *argv[])
{
    Imgs imgs;
    argparse(argc, argv); 
    print_args();

    init_convas(options.rows, options.cols);
    open_imgs(&imgs, options.rows, options.cols, options.dir);
    
    img_stitch(&imgs, options.rows, options.cols, options.out);    

    close_imgs(&imgs, options.rows, options.cols);
    free_convas();
    return 0;
}