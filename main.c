#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>


struct commands {
    bool all_files;
    bool show_bytes;
    bool block_size;
    int block_size_bytes; 
    bool dereference;
    bool separate_dirs;
    bool max_depth;
    int max_depth_size;
} coms;



int main(int argc, char* argv[]){
    if(argc < 2){ //2 ou 1, depende se é preciso especificar ou não o path
        perror("Wrong number of arguments");
        exit(1);
    }

    coms.all_files = false;
    coms.show_bytes = false;
    coms.block_size = false;
    coms.block_size_bytes = 0;
    coms.separate_dirs = false;
    coms.max_depth = false;
    coms.max_depth_size = 0;

    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--all") == 0){
            coms.all_files = true;
        }

        if(strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--bytes")){
            coms.show_bytes = true;
        }

        if(strcmp(argv[i], "-B") == 0){
            coms.block_size = true;
            coms.block_size_bytes = atoi(argv[i + 1]);
        }

        if(strncmp(argv[i], "--block-size=", 13)){
            coms.block_size = true;
            //Descobrir numero de bytes neste caso
        }        

        if(strcmp(argv[i], "-S") == 0 || strcmp(argv[i], "--separate-dirs")){
            coms.separate_dirs = true;
        }

        if(strncmp(argv[i], "--max-depth=", 12) == 0){
            coms.max_depth = true;
            //Descobrir a profundidade neste caso
        }
    }
}