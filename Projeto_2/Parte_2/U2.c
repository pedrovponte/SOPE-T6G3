#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "args_handler.h"
#include "utils.h"

int fd;
char fifo_name[50];
int finish = 0;

void *sendFifo(void * number){

    int fd2;
    Pedido request;

    char private_fifo[50];
    sprintf(private_fifo, "/tmp/%d.%ld", getpid(), pthread_self());

    if(mkfifo(private_fifo, 0660) < 0){
        perror("Error creating FIFO");
        exit(1);
    }

    request.id = *(int *) number;
    request.dur = rand() % 3000001 + 1;
    request.pid = getpid();
    request.tid = pthread_self();
    request.pl = -1;

    int i  = write(fd, &request, sizeof(Pedido));

    if(i == -1){
        perror("Error writing to fifo.");
        pthread_exit(NULL);
    }
    else {
        registLog(request.id, request.pid, request.tid, request.dur, request.pl, "IWANT");
    }

    if((fd2 = open(private_fifo, O_RDONLY /*| O_NONBLOCK*/)) == -1){
        perror("Error opening FIFO");
        exit(1);
    }

    i = read(fd2, &request, sizeof(Pedido));

    if(i < 0){
        perror("ERROR reading from FIFO");
        pthread_exit(NULL);
    }
    else if(i > 0){
        if(request.pl > 0 && request.dur > 0){
            registLog(request.id, request.pid, request.tid, request.dur, request.pl, "IAMIN");
        }
        else {
            registLog(request.id, request.pid, request.tid, request.dur, request.pl, "CLOSD");
        }
    }
    else{
        registLog(request.id, request.pid, request.tid, request.dur, request.pl, "FAILD");
    }

    if(close(fd2) == -1){
        perror("Error closing fifo.");
        pthread_exit(NULL);
    }

    if(unlink(private_fifo) == -1) {
        perror("ERROR unlinking FIFO");
        exit(1);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){

    if(argc < 4){
        perror("Wrong number of arguments");
        print_usage_u();
        exit(1);
    }

    args_u1 args = process_args_u(argc, argv);

    int id = 1, tents = 0;

    int max_time = time(NULL) + args.nsecs;

    do {
        fd = open(args.fifoname, O_WRONLY);
        if (fd == -1) {
            printf("Connecting to server...\n");
            tents++;
            sleep(1);
        }
    } while((fd == -1) && tents < 5);

    if(tents == 5) {
        perror(" Main ERROR opening FIFO");
        exit(1);
    }

    while(time(NULL) < max_time){
        pthread_t tid;
        if(pthread_create(&tid, NULL, sendFifo, (void *) &id)){
            perror("ERROR creating thread");
            exit(1);
        }

        pthread_detach(tid);

        if(usleep(10000)) {
            perror("ERROR sleeping");
            exit(1);
        }
        id++;
    }

    if(close(fd) == -1) {
        perror("ERROR closing FIFO");
        exit(1);
    }

    pthread_exit(NULL);
}
