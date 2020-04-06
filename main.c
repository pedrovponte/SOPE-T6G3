#include <stdio.h>
#include <stdlib.h>
#include "args_handler.h"
#include "dirs.h"
#include "logs.h"

char *directory;
int fd[2];

int main(int argc, char *argv[]){
    struct commands coms;
    char *copy = malloc(100);
    get_initial_time();

    if(argc < 1){ 
        fprintf(stderr, "Wrong number of arguments.\n");
        exit(1);
    }

    if(argc > 1){
        if(strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--all") == 0 || strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "--bytes") == 0 || strcmp(argv[1], "-B") == 0 || strncmp(argv[1], "--block-size=", 13) == 0 || strcmp(argv[1], "-S") == 0 || strcmp(argv[1], "--separate-dirs") == 0 || strncmp(argv[1], "--max-depth=", 12) == 0){
            strcpy(copy, ".");
            directory = ".";
        }
        else{
            if(check_is_dir(argv[1])){
                strcpy(copy, argv[1]);
                directory = argv[1];
            }
        }
    }
    else{
        strcpy(copy,".");
        directory = ".";
    }

    if (copy[strlen(copy) - 1] != '/') {
        copy[strlen(copy)] = '/';
        copy[strlen(copy) + 1] = '\0';
    }

    //printf("%s\n", copy);

    args_commands(argc, argv, &coms);

    pipe(fd);

    dirs(&coms, copy, 0);

    return 0;
}