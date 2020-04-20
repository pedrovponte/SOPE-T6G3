#include "args_handler.h"

extern char* copy;
extern char* act_dir;
extern bool inp_dir;

commands args_commands(int argc, char* argv[]){
    commands coms;
    
    coms.all_files = 0;
    coms.show_bytes = 0;
    coms.block_size = 0;
    coms.block_size_bytes = 1024;
    coms.separate_dirs = 0;
    coms.max_depth = 0;
    coms.max_depth_size = INT_MAX;

    inp_dir = false;

    for(int i = 1; i < argc; i++){

        if(strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--all") == 0){
            coms.all_files = 1;
        }

        else if(strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--bytes") == 0){
            coms.show_bytes = 1;
        }

        else if(strcmp(argv[i], "-B") == 0){
            i++;
            coms.block_size = 1;
            coms.block_size_bytes = atoi(argv[i]);
        }

        else if(strncmp(argv[i], "--block-size=", 13) == 0){
            coms.block_size = 1;
            char str[16];
            char* last;
            strcpy(str, argv[i]);
            char* token = strtok(str, "=");
            while(token != NULL){
                last = token;
                token = strtok(NULL, "=");
            }
            coms.block_size_bytes = atoi(last);
        }   

        else if(strcmp(argv[i], "-l") == 0  || strcmp(argv[i], "--count-links") == 0){
            continue;
        }

        else if(strcmp(argv[i], "-L") == 0 || strcmp(argv[i], "--dereference") == 0){
            coms.dereference = 1;
        }     

        else if(strcmp(argv[i], "-S") == 0 || strcmp(argv[i], "--separate-dirs") == 0){
            coms.separate_dirs = 1;
        }

        else if(strncmp(argv[i], "--max-depth=", 12) == 0){
            coms.max_depth = 1;
            char str[16];
            char* last;
            strcpy(str, argv[i]);
            char* token = strtok(str, "=");
            while(token != NULL){
                last = token;
                token = strtok(NULL, "=");
            }
            coms.max_depth_size = atoi(last);
        }

        else if(check_is_dir(argv[i])){
            inp_dir = true;
            strcpy(copy, argv[i]);
            act_dir = argv[i];
        }

        else{
            perror("Invalid argument");
        }
    }
    if(!inp_dir){
        strcpy(copy, ".");
        act_dir = ".";
    }

    if (copy[strlen(copy) - 1] != '/') {
        copy[strlen(copy)] = '/';
        copy[strlen(copy) + 1] = '\0';
    }

    return coms;
}

