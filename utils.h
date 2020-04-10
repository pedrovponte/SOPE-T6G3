#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logs.h"
#include "stdbool.h"

extern char* act_dir;

bool check_is_dir(const char* path);
int print_screen(const char* path, long int size);
