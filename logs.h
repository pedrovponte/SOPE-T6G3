#pragma once

#include <sys/types.h>
#include <unistd.h>

int get_initial_time();

int openLog(const char* filename);

double get_elapsed_time(void);

int registLog(pid_t pid, char* action, char* info);

int closeLog();