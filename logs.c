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

int registLog(pid_t pid, char* action, char *info){
    if(fprintf(file, " %2f \t-\t %8u \t-\t %s \t-\t %s\n", get_elapsed_time(), pid, action, info) < 0){
        perror("Error writing to log file.\n");
        exit(1);
    }

    fflush(file);

    return 0;
}

void createLog(int argc, char *args[]){
    //pid_t pid = getpid();
    char info[150];
    for(int i = 0; i < argc; i++){
        strcat(info, args[i]);
        while(i != argc - 1){
            strcat(info, " ");
        }
    }
    registLog(getpid(), "CREATE", info);
}

void exitLog(int status_code){
    //pid_t pid = getpid();
    char info[150];
    sprintf(info, "%d", status_code);
    registLog(getpid(), "EXIT", info);
}

void recvSignalLog(char *signal){
    //pid_t pid = getpid();
    char info[150];
    strcpy(info, signal);
    registLog(getpid(), "RECV_SIGNAL", info);
}

void sendSignalLog(char *signal, int pid){
    //pid_t pidp = getpid();
    char info[200];
    sprintf(info, "Signal %s sent to process with pid %d", signal, pid);
    registLog(getpid(), "SEND_SIGNAL", info);
}

void recvPipeLog(long int message){
    char info[200];
    sprintf(info, "%ld", message);
    registLog(getpid(), "RECV_PIPE", info);
}

void sendPipeLog(long int message){
    char info[200];
    sprintf(info, "%ld", message);
    registLog(getpid(), "SEND_PIPE", info);
}

void entryLog(char *path){
    //pid_t pid = getpid();
    char info[200];
    sprintf(info, "%s", path);
    registLog(getpid(), "ENTRY", info);
}

int closeLog(){
    if(fclose(file) != 0){
        perror("Error closing log file.\n");
        exit(1);
    }
    return 0;
}