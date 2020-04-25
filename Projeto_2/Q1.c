#include <stdio.h>
#include "args_handler.h"
#include "logs.h"

int current_time;
int max_time;

int main(int argc, char *argv[]){

    if(argc < 4){ 
        perror("Wrong number of arguments.");
        exit(1);
    }

    args_q1 args = process_args_q(argc, argv);

    int current_time = 0;
    int max_time = args.nsecs;
    
    

}