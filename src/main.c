#include <stdio.h>
#include <stdlib.h>
#include "argparse.h"

int main(int argc, char *argv[])
{
    argparse(argc, argv); 
    print_args();
    return 0;
}