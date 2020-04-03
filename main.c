#include <stdio.h>
#include <stdlib.h>
#include "args_handler.h"
#include "dirs.h"
#include "logs.h"


int main(int argc, char *argv[]){
    struct commands coms;
    //const char *path;
    char *copy = malloc(100);
    //int i = 0;

    if(argc < 1){ 
        fprintf(stderr, "Wrong number of arguments.\n");
        exit(1);
    }

    if(argc > 1){
        if(strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--all") == 0 || strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "--bytes") == 0 || strcmp(argv[1], "-B") == 0 || strncmp(argv[1], "--block-size=", 13) == 0 || strcmp(argv[1], "-S") == 0 || strcmp(argv[1], "--separate-dirs") == 0 || strncmp(argv[1], "--max-depth=", 12) == 0){
            strcpy(copy, ".");
        }
        else{
            if(check_is_dir(argv[1])){
                /*path = argv[1];
                while(path[i] != '/' && path[i] != '\0'){
                    i++;
                }
                if(path[i] == '/'){
                    strncpy(copy, path + i + 1, 100);
                }
                else{
                    strcpy(copy, path);
                }*/
                strcpy(copy, argv[1]);
                if(strncmp(copy, "./", 2) == 0){
                    copy = strtok(copy, "./");
                }
            }
        }
    }
    else{
        strcpy(copy,".");
    }

    

    printf("%s\n", copy);

    args_commands(argc, argv, &coms);

    dirs(&coms,copy);

    return 0;
}