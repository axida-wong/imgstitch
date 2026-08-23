#include <stdio.h>
#include <stdlib.h>
#include "argparse.h"
#include "imgstitch.h"

int main(int argc, char *argv[])
{
    Imgs imgs;
    argparse(argc, argv); 
    print_args();

    init_imgs(&imgs, &options);
    open_imgs(&imgs);
    
    TIFF *stitched = img_stitch(&imgs);    
    TIFFClose(stitched);

    close_imgs(&imgs);
    return 0;
}