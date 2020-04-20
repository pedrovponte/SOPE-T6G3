#include <string.h>
#include <stdlib.h>

typedef struct {
    int nsecs;
    char fifoname[200];
} args_u1;

typedef struct {
    int nsecs;
    int nplaces;
    int nthreads;
    char fifoname[200];
} args_q1;

void print_usage_u();

args_u1 process_args_u(int argc, char *argv[]);

void print_usage_q();

args_q1 process_args_q(int argc, char *argv[]);