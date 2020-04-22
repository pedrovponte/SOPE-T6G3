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

    request.i = *(int *) number;
    request.dur = rand() % 3000001 + 1;
    request.pid = getpid();
    request.tid = pthread_self();
    request.pl = -1;

    write(fd, &request, sizeof(request));

    char *private_fifo = construct_fifo_name(request.pid, request.tid);
    mkfifo(private_fifo, 0660);

    fd2 = open(private_fifo, O_RDONLY);






    free(fd2);
    free(private_fifo);
}

int main(int argc, char *argv[]){

    if(argc < 4){ 
        perror("Wrong number of arguments.");
        exit(1);
    }

    args_u1 args = process_args_u(argc, argv);

    int i = 1;
    int current_time = 0;

    strcpy(fifo_name, args.fifoname)

    while(fd != -1){
        fd = open(fifo_name, O_WRONLY);
    }

    while(current_time < args.nsecs){
        pthread_t tid;
        pthread_create(&tid, NULL, sendFifo, (void *) &i);
        i++;
        current_time += 300000;
        usleep(300000);
    }

    pthread_exit(0);
    close(fd);

    return 0;
}