#ifndef ARGPARSE_H
#define ARGPARSE_H

#define _GNU_SOURCE

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct opts
{
    int rows;
    int cols;
    int height;
    int width;
    char *dir;
    char *out;
}Opts;

extern Opts options;

void argparse(int argc, char *argv[]);

void print_args(void);

#endif