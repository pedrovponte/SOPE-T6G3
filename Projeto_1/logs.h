#pragma once

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>

int get_initial_time();

int openLog();

double get_elapsed_time(void);

int registLog(pid_t pid, char* action, char* info);

void createLog(int argc, char *args[]);

void exitLog(int status_code);

void recvSignalLog(char *signal);

void sendSignalLog(char *signal, pid_t pid);

void recvPipeLog(long int message);

void sendPipeLog(long int message);

void entryLog(char *path);

int closeLog();