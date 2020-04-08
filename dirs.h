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
#include <signal.h>
#include "args_handler.h"
#include "logs.h"

int dirs(char *path_file, int level);