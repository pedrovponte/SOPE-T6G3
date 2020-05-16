#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int nsecs;
    char fifoname[200];
} args_u2;

typedef struct {
    int nsecs;
    int nplaces;
    int nthreads;
    char fifoname[200];
} args_q2;

void print_usage_u();

args_u2 process_args_u(int argc, char *argv[]);

void print_usage_q();

args_q2 process_args_q(int argc, char *argv[]);
