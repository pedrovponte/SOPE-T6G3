#include <stdio.h>
#include "args_handler.h"
#include "logs.h"

int main(int argc, char *argv[]){

    if(argc < 4){ 
        perror("Wrong number of arguments.");
        exit(1);
    }

    args_u1 args = process_args_u(argc, argv);
}