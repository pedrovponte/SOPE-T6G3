#pragma once

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>
#include <pthread.h>

int get_initial_time();

int openLog();

double get_elapsed_time(void);

int registLog(int i, pid_t pid, pthread_t tid, int dur, int pl, char* action);

void iwantLog();

void recvdLog();

void enterLog();

void iaminLog();

void timupLog();

void late_2_Log();

void closdLog();

void faildLog();

void gavupLog();

int closeLog();