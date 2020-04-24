#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "args_handler.h"
#include "logs.h"
#include "utils.h"

int fd;
char fifo_name[50];

void *sendFifo(void * number){

    int fd2;
    pedido request;

    request.id = *(int *) number;
    request.dur = rand() % 3000001 + 1;
    request.pid = getpid();
    request.tid = pthread_self();
    request.pl = -1;

    if(write(fd, &request, sizeof(pedido)) == -1){
        perror("Error writing to fifo.");
        exit(1);
    } 

    char private_fifo[50];
    sprintf(private_fifo, "/tmp/%d.%d", request.pid, request.tid);
    
    if(mkfifo(private_fifo, 0660) != 0){
        perror("Error creating fifo.");
        exit(1);
    }

    if((fd2 = open(private_fifo, O_RDONLY | O_NONBLOCK)) != 0){
        perror("Error opening fifo.");
        exit(1);
    }

    pedido answer;

    while(read(fd2, &answer, sizeof(pedido))){
        usleep(15000);
    }

    
    //fazer operacoes de escrita

    if(close(fd2) == -1){
        perror("Error closing fifo.");
        exit(1);
    }

    
    unlink(private_fifo);
    return 0;
}

int main(int argc, char *argv[]){

    if(argc < 4){ 
        perror("Wrong number of arguments.");
        exit(1);
    }

    args_u1 args = process_args_u(argc, argv);

    int id = 1;
    int current_time = 0;

    strcpy(fifo_name, args.fifoname);

    do {
        fd = open(fifo_name, O_WRONLY);
        if(fd == -1){
            printf("Connecting to server...\n");
            sleep(1);
        }
    } while(fd == -1);

    while(current_time < (args.nsecs * 1000000)){
        int rc;
        pthread_t tid;
        rc = pthread_create(&tid, NULL, sendFifo, (void *) &id);
        if(rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(1);
        }
        id++;
        current_time += 300000;
        usleep(300000);
    }

    pthread_exit(0);
    
    if(close(fd) == -1){
        perror("Error closing fifo.");
        exit(1);
    }

    return 0;
}