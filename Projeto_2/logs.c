#include "logs.h"

struct timeval initial;


int get_initial_time(){
    if(gettimeofday(&initial, NULL) != 0){
        perror("Error getting initial time.\n");
        exit(1);
    }

    return 0;
}

double get_elapsed_time(void){
    struct timeval current;

    if(gettimeofday(&current, NULL) != 0){
        perror("Error getting current time.\n");
        exit(1);
    }

    float elapsed_time = (float) ((current.tv_usec - initial.tv_usec) / 1000);

    return elapsed_time;
}

int registLog(int i, pid_t pid, pthread_t tid, int dur, int pl, char* action){
    char* log = malloc(500);
    time_t inst = time(NULL);
    sprintf(log, " %ld; %i; %.8u; %.8lu; %i; %i; %s\n", inst, i, pid, tid, dur, pl, action);
    write(STDOUT_FILENO, log, strlen(log));

    return 0;
}
