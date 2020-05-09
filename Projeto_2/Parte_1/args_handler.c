#include "args_handler.h"

void print_usage_u(){
    printf("%s", "USAGE: Un <-t nsecs> fifoname\n");
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
            printf("%s\n", "ERROR: Invalid argument");
            print_usage_u();
            exit(1);
        }
    }

    if(args.nsecs <= 0){
        printf("%s\n", "ERROR: Invalid number of seconds");
        print_usage_u();
        exit(1);
    }

    return args;
}

void print_usage_q(){
    printf("%s", "USAGE: Qn <-t nsecs> fifoname");
}

args_q1 process_args_q(int argc, char *argv[]){
    args_q1 args;
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
            printf("%s\n", "ERROR: Invalid argument");
            print_usage_q();
            exit(1);
        }
    }

    if(args.nsecs <= 0){
        printf("%s\n", "ERROR: Invalid number of seconds");
        print_usage_q();
        exit(1);
    }

    return args;
}
