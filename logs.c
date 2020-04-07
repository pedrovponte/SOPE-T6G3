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

    if((file = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666)) == NULL){ //ver no caso de o ficheiro ser dado na linha de comandos ou ser um a nossa escolha
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
    if(fprintf(file, " %2f - %8u - %s - %s\n", get_elapsed_time(), pid, action, info) < 0){
        perror("Error writing to log file.\n");
        exit(1);
    }

    fflush(file);

    return 0;
}

void createLog(int argc, char *args[]){
    pid_t pid = getpid();
    char info[150];
    for(int i = 0; i < argc; i++){
        strcat(info, args[i]);
        while(i != argc - 1){
            strcat(info, " ");
        }
    }
    registLog(pid, "CREATE", info);
}

void exitLog(int status_code){
    pid_t pid = getpid();
    char info[150];
    sprintf(info, "%d", status_code);
    registLog(pid, "EXIT", info);
}

void recvSignalLog(char *signal){
    pid_t pid = getpid();
    char info[150];
    strcpy(info, signal);
    registLog(pid, "RECV_SIGNAL", info);
}

void sendSignalLog(char *signal, int pid){
    pid_t pidp = getpid();
    char info[200];
    sprintf(info, "Signal %s sent to process with pid %d", signal, pid);
    registLog(pidp, "SEND_SIGNAL", info);
}

void recvPipeLog(char *message){
    pid_t pid = getpid();
    char info[200];
    sprintf(info, "%s", message);
    registLog(pid, "RECV_PIPE", info);
}

void sendPipeLog(char *message){
    pid_t pid = getpid();
    char info[200];
    sprintf(info, "%s", message);
    registLog(pid, "SEND_PIPE", info);
}

void entryLog(char *path){
    pid_t pid = getpid();
    char info[200];
    sprintf(info, "%s", path);
    registLog(pid, "ENTRY", info);
}

int closeLog(){
    if(fclose(file) != 0){
        perror("Error closing log file.\n");
        exit(1);
    }
    return 0;
}