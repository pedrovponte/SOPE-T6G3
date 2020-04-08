#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <inttypes.h>

typedef struct{
    int all_files;
    int show_bytes;
    int block_size;
    int block_size_bytes; 
    int dereference;
    int separate_dirs;
    int max_depth;
    long int max_depth_size;
} commands;

commands args_commands(int argc, char* argv[]);

bool check_is_dir(const char* path);