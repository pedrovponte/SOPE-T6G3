#pragma once

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int get_initial_time();

int openLog(const char* filename);

double get_elapsed_time(void);

int registLog(pid_t pid, char* action, char* info);

void createLog(int argc, char *args[]);

void exitLog(int status_code);

void recvSignalLog(char *signal);

void sendSignalLog(char *signal, int pid);

void recvPipeLog(char *message);

void sendPipeLog(char *message);

void entryLog(char *path);

int closeLog();