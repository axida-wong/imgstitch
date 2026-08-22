#define _GNU_SOURCE

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

static void print_help_menu(char *prog_name);

typedef struct opts
{
    int rows;
    int cols;
    int height;
    int width;
    char *dir;
    char *out;
}Opts;

static const struct option longopts[] = 
{
    {"rows", 1, NULL, 'r'},
    {"cols", 1, NULL, 'c'},
    {"height", 1, NULL, 'h'},
    {"width", 1, NULL, 'w'},
    {"dir", 1, NULL, 'd'},
    {"out", 1, NULL, 'o'},
    {0,0,0,0}
};

Opts options = {0};

void argparse(int argc, char *argv[])
{
    options.out = ".";

    if (argc < 2)
    {
        print_help_menu(*argv);
        exit(EXIT_FAILURE);
    }
    int opt;
    while ((opt = getopt_long(argc, argv, ":r:c:h:w:d:o:", longopts, NULL)) != -1)
    {
        switch (opt)
        {
            case 'r':
                options.rows = atoi(optarg);
                break;
            case 'c':
                options.cols = atoi(optarg);
                break;
            case 'h':
                options.height = atoi(optarg);
                break;
            case 'w':
                options.width = atoi(optarg);
                break;
            case 'd':
                options.dir = optarg;
                break;
            case 'o':
                options.out = optarg;
                break;
            case ':':
                fprintf(stderr, "option needs a value\n");
                exit(EXIT_FAILURE);
            case '?':
                fprintf(stderr, "Unknown option %c\n", optopt);
                exit(EXIT_FAILURE);
            default:
        }
    }
}

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