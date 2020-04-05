#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>

struct timeval initial;
FILE *file;

int get_initial_time(){
    if(gettimeofday(&initial, NULL) != 0){
        perror("Error getting initial time.\n");
        exit(1);
    }

    return 0;
}

int openLog(const char* filename){
    get_initial_time();

    if((file = fopen(filename, "a")) == NULL){ //ver no caso de o ficheiro ser dado na linha de comandos ou ser um a nossa escolha
        perror("Error opening log file.\n");
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

int registLog(pid_t pid, char* action, long int info){
    if(fprintf(file, " %2f - %8u - %s - %li\n", get_elapsed_time(), pid, action, info) < 0){
        perror("Error writing to log file.\n");
        exit(1);
    }

    fflush(file);

    return 0;
}

int closeLog(){
    if(fclose(file) != 0){
        perror("Error closing log file.\n");
        exit(1);
    }
    return 0;
}