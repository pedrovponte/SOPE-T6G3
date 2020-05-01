#include "utils.h"

int registLog(int i, pid_t pid, pthread_t tid, int dur, int pl, char* action){
    char* log = malloc(500);
    time_t inst = time(NULL);
    sprintf(log, " %ld; %i; %.8u; %.8lu; %i; %i; %s\n", inst, i, pid, tid, dur, pl, action);
    write(STDOUT_FILENO, log, strlen(log));

    return 0;
}
