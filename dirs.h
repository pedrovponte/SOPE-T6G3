#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <sys/wait.h>
#include "args_handler.h"

int dirs(struct commands *coms, const char *path_file);