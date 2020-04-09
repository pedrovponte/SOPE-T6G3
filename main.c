#include <stdio.h>
#include <stdlib.h>
#include "args_handler.h"
#include "dirs.h"
#include "logs.h"
#include "utils.h"

char *directory;
int fd[2];
char *copy;
commands coms;

int main(int argc, char *argv[]){
    copy = malloc(100);
    openLog();

    if(argc < 1){ 
        fprintf(stderr, "Wrong number of arguments.\n");
        exit(1);
    }

    coms = args_commands(argc, argv);


    createLog(argc, argv);

    //args_commands(argc, argv);

    pipe(fd);

    dirs(copy, 0);

    exitLog(0);
    return 0;
}