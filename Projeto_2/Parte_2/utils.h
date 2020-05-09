#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>
#include <fcntl.h>
#include <pthread.h>

typedef struct {
    int id;
    int pid;
    int tid;
    int dur;
    int pl;
} Pedido;

int registLog(int i, pid_t pid, pthread_t tid, int dur, int pl, char* action);
