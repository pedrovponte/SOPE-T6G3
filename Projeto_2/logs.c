#include "logs.h"

struct timeval initial;
FILE *file;

int get_initial_time(){
    if(gettimeofday(&initial, NULL) != 0){
        perror("Error getting initial time.\n");
        exit(1);
    }

    return 0;
}

int openLog(){
    get_initial_time();
    setenv("LOG_FILENAME", "logfile.txt", 0);
    
    if((file = fopen(getenv("LOG_FILENAME"), "w")) == NULL){ 
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

int registLog(int i, pid_t pid, pthread_t tid, int dur, int pl, char* action){
    if(fprintf(file, " %.2f \t;\t %i \t;\t %.8u \t;\t %.8lu \t;\t %i \t;\t %i \t;\t %s\n", get_elapsed_time(), i, pid, tid, dur, pl, action) < 0){
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