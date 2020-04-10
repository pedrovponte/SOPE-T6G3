#include <stdio.h>
#include <stdlib.h>
#include "args_handler.h"
#include "dirs.h"
#include "logs.h"
#include "utils.h"
#include "signals_handler.h"

char *act_dir;
int fd[2];
char *copy;
commands coms;
pid_t pgid;

int main(int argc, char *argv[]){
    
    copy = malloc(512);

    if(init_signals() != 0){
        perror("Error initializing signals");
        exitLog(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }
    
    if(openLog() !=0){
        perror("Error opening logs file");
        exitLog(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    if(argc < 1){ 
        fprintf(stderr, "Wrong number of arguments.\n");
        exitLog(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    coms = args_commands(argc, argv);

    createLog(argc, argv);

    if(pipe(fd) < 0){
        perror("Error creating pipe");
        exitLog(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    dirs(copy, 0);

    exitLog(EXIT_SUCCESS);
    
    return 0;
}