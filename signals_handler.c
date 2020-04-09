#include "signals_handler.h"

extern pid_t pgid; 

void sigint_handler(){
    recvSignalLog("SIGINT");
    pid_t pid = getpid();
    char buf[10];

    while(1){
        printf("Are you sure you want to finish the program? (Y / N) \n");
        scanf("%s", buf);

        if(strcmp("Y", buf) == 0 || strcmp("y", buf) == 0){
            sendSignalLog("SIGTERM", pid);
            kill(-pid, SIGTERM);
            break;
        }
        else if(strcmp("N", buf) == 0 || strcmp("n", buf) == 0){
            sendSignalLog("SIGCONT", pid);
            kill(-pid, SIGCONT);
            break;       
        }
        else{
            printf("Invalid answer\n");
        }
    }
}

void sigterm_handler(){
    recvSignalLog("SIGTERM");
    printf("Finishing processes...\n");
    exitLog(EXIT_FAILURE);
    exit(EXIT_FAILURE);
}

void sigcont_handler(){
    recvSignalLog("SIGCONT");
    printf("Continuing processes...\n");
}


int init_signals(){
    struct sigaction act_sigint, act_sigterm, act_sigcont;

    pgid = getpgrp();
    act_sigint.sa_handler = sigint_handler;
    sigemptyset(&act_sigint.sa_mask);
    act_sigint.sa_flags = 0;

    if(sigaction(SIGINT, &act_sigint, NULL) < 0){
        fprintf(stderr, "Unable to install SIGINT handler\n");
        exit(1);
    }

    act_sigterm.sa_handler = sigterm_handler;
    sigemptyset(&act_sigterm.sa_mask);
    act_sigterm.sa_flags = 0;

    if(sigaction(SIGTERM, &act_sigterm, NULL) < 0){
        fprintf(stderr, "Unable to install SIGTERM handler\n");
        exit(1);
    }

    act_sigcont.sa_handler = sigcont_handler;
    sigemptyset(&act_sigcont.sa_mask);
    act_sigcont.sa_flags = 0;

    if(sigaction(SIGCONT, &act_sigcont, NULL) < 0){
        fprintf(stderr, "Unable to install SIGCONT handler\n");
        exit(1);
    }
    
    return 0;
}