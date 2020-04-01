#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <inttypes.h>

struct commands {
    bool all_files;
    bool show_bytes;
    bool block_size;
    int block_size_bytes; 
    bool dereference;
    bool separate_dirs;
    bool max_depth;
    long int max_depth_size;
};

int args_commands(int argc, char* argv[], struct commands *coms);

bool check_is_dir(const char* path);