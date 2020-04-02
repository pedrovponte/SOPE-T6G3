#include "args_handler.h"


int args_commands(int argc, char* argv[], struct commands *coms){

    coms->all_files = false;
    coms->show_bytes = false;
    coms->block_size = false;
    coms->block_size_bytes = 0;
    coms->separate_dirs = false;
    coms->max_depth = false;
    coms->max_depth_size = 0;

    for(int i = 1; i < argc; i++){

        if(strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--all") == 0){
            coms->all_files = true;
        }

        if(strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--bytes") == 0){
            coms->show_bytes = true;
        }

        if(strcmp(argv[i], "-B") == 0){
            coms->block_size = true;
            coms->block_size_bytes = atoi(argv[i + 1]);
        }

        if(strncmp(argv[i], "--block-size=", 13) == 0){
            coms->block_size = true;
            char str[16];
            char* last;
            strcpy(str, argv[i]);
            char* token = strtok(str, "=");
            while(token != NULL){
                last = token;
                token = strtok(NULL, "=");
            }
            coms->block_size_bytes = atoi(last);
        }        

        if(strcmp(argv[i], "-S") == 0 || strcmp(argv[i], "--separate-dirs") == 0){
            coms->separate_dirs = true;
        }

        if(strncmp(argv[i], "--max-depth=", 12) == 0){
            coms->max_depth = true;
            char str[16];
            char* last;
            strcpy(str, argv[i]);
            char* token = strtok(str, "=");
            while(token != NULL){
                last = token;
                token = strtok(NULL, "=");
            }
            coms->max_depth_size = atoi(last);
        }
    }
    return 0;
}

bool check_is_dir(const char* path){
    struct stat path_stat;

    if(stat(path, &path_stat) != 0){
        fprintf(stderr, "Invalid directory.\n");
        exit(1);
    }

    return S_ISDIR(path_stat.st_mode);
}