#include <stdio.h>
#include <stdlib.h>
#include "args_handler.h"
#include "dirs.h"
#include "logs.h"

char *directory;
int fd[2];
char *copy;

int main(int argc, char *argv[]){
    struct commands coms;
    copy = malloc(100);
    //openLog(getenv("LOG_FILENAME"));

    if(argc < 1){ 
        fprintf(stderr, "Wrong number of arguments.\n");
        exit(1);
    }

    if(argc == 1){
        strcpy(copy,".");
        directory = ".";
    }
    else{
        args_commands(argc, argv, &coms);
    }

    //createLog(argc, argv);

    args_commands(argc, argv, &coms);

    pipe(fd);

    dirs(&coms, copy, 0);

    return 0;
}