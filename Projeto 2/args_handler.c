#include "args_handler.h"

void print_usage_u(){
    printf("USAGE: Un <-t nsecs> fifoname\n");
}

args_u1 process_args_u(int argc, char *argv[]){
    args_u1 args;
    args.nsecs = 0;
    strcpy(args.fifoname, "");

    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-t") == 0){
            i++;
            int secs = atoi(argv[i]);
            args.nsecs = secs;
        }

        else if(strcmp(args.fifoname, "") == 0){
            strcpy(args.fifoname, argv[i]);
        }

        else{
            perror("Invalid argument");
            print_usage_u();
            exit(1);
        }
    }

    if(args.nsecs == 0){
        perror("Invalid number of seconds");
        print_usage_u();
        exit(1);
    }

    return args;
}

void print_usage_q(){
    printf("USAGE: Qn <-t nsecs> [-l nplaces] [-n nthreads] fifoname");
}

args_q1 process_args_q(int argc, char *argv[]){
    args_q1 args;
    args.nsecs = 0;
    args.nplaces = __INT_MAX__;
    args.nthreads = __INT_MAX__;
    strcpy(args.fifoname, "");

    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-t") == 0){
            i++;
            int secs = atoi(argv[i]);
        }

        else if(strcmp(argv[i], "-l") == 0){
            i++;
            int pls = atoi(argv[i]);
        }

        else if(strcmp(argv[i], "-n") == 0){
            i++;
            int ths = atoi(argv[i]);
        }

        else if(strcmp(args.fifoname, "") == 0){
            strcpy(args.fifoname, argv[i]);
        }

        else{
            perror("Invalid argument");
            print_usage_q();
            exit(1);
        }
    }

    if(args.nsecs == 0){
        perror("Invalid number of seconds");
        print_usage_u();
        exit(1);
    }

    return args;
}