#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include <inttypes.h>
//#include "logs.h"
//#include "dirs.h"


struct commands {
    bool all_files;
    bool show_bytes;
    bool block_size;
    int block_size_bytes; 
    bool dereference;
    bool separate_dirs;
    bool max_depth;
    long int max_depth_size;
} coms;



int main(int argc, char* argv[]){

intmax_t num;
char* end;
int base = 10;

    if(argc < 2){ //2 ou 1, depende se é preciso especificar ou não o path
        fprintf(stderr, "Wrong number of arguments.\n");
        return 1;
    }

    coms.all_files = false;
    coms.show_bytes = false;
    coms.block_size = false;
    coms.block_size_bytes = 0;
    coms.separate_dirs = false;
    coms.max_depth = false;
    coms.max_depth_size = 0;

    for(int i = 1; i < argc; i++){

        printf("%s\n", argv[i]);

        if(strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--all") == 0){
            coms.all_files = true;
        }

        if(strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--bytes") == 0){
            coms.show_bytes = true;
        }

        if(strcmp(argv[i], "-B") == 0){
            coms.block_size = true;
            coms.block_size_bytes = atoi(argv[i + 1]);
        }

        if(strncmp(argv[i], "--block-size=", 13) == 0){
            coms.block_size = true;
            num = strtoimax(argv[i], &end, base);
            coms.block_size_bytes = num;
        }        

        if(strcmp(argv[i], "-S") == 0 || strcmp(argv[i], "--separate-dirs") == 0){
            coms.separate_dirs = true;
        }

        if(strncmp(argv[i], "--max-depth=", 12) == 0){
            coms.max_depth = true;
            num = strtoimax(argv[i], &end, base);
            coms.max_depth_size = num;
        }
    }

    printf("%d, %d, %d, %i, %d, %d, %li \n",coms.all_files, coms.show_bytes, coms.block_size, coms.block_size_bytes, coms.separate_dirs, coms.max_depth, coms.max_depth_size);
    return 0;
}