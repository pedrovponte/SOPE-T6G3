#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "args_handler.h"
#include "utils.h"


int max_time = 0;

void * processFifo(void *req) {

    Pedido pedido = *(Pedido *) req;
    registLog(pedido.id, pedido.pid, pedido.tid, pedido.dur, pedido.pl, "RECVD");

    char private_fifo[50];
    sprintf(private_fifo, "/tmp/%d.%ld", pedido.pid, pedido.tid);

    int fd2;
    if((fd2 = open(private_fifo, O_WRONLY)) == -1) {
        perror("ERROR opening FIFO");
        exit(1);
    }

    if(time(NULL) < max_time) {
        registLog(pedido.id, pedido.pid, pedido.tid, pedido.dur, pedido.pl, "ENTER");
        usleep(pedido.dur);
        registLog(pedido.id, pedido.pid, pedido.tid, pedido.dur, pedido.pl, "TIMUP");
    }
    else{
        pedido.pl = -1;
        pedido.dur = -1;
        registLog(pedido.id, pedido.pid, pedido.tid, pedido.dur, pedido.pl, "2LATE");
    }

    int i = write(fd2, &pedido, sizeof(Pedido));

    if(i < 0) {
        perror("ERROR writing to FIFO");
        pthread_exit(NULL);
    }

    else if (i == 0) {
        registLog(pedido.id, pedido.pid, pedido.tid, pedido.dur, pedido.pl, "GAVUP");
    }

    if(close(fd2)){
        perror("Error closing fifo.");
        exit(1);
    }

    pthread_exit(NULL);
}


int main(int argc, char *argv[]){

    if(argc < 4){
        perror("Wrong number of arguments.");
        print_usage_q();
        exit(1);
    }

    int fd1, place = 0, i = 0;
    args_q1 args = process_args_q(argc, argv);

    max_time = time(NULL) + args.nsecs;

    unlink(args.fifoname);
    if(mkfifo(args.fifoname, 0660) == -1) {
        perror("ERROR creating FIFO");
        exit(1);
    }

    if((fd1 = open(args.fifoname, O_RDONLY /*| O_NONBLOCK*/)) == -1) {
        perror("Main ERROR opening FIFO");
        exit(1);
    }

    Pedido pedido;

    while(time(NULL) < max_time){
        i = read(fd1, &pedido, sizeof(Pedido));
        if(i < 0 /* && time(NULL) < max_time*/){
            /*printf("%s\n", "Waiting for requests...");
            sleep(1);*/
            perror("ERROR reading FIFO");
            exit(1);
        }
        else if(i > 0) {
          place++;
          pedido.pl = place;
          pthread_t tid;
          if(pthread_create(&tid, NULL, processFifo, (void *) &pedido)) {
              perror("ERROR creating thread");
              exit(1);
          }
          pthread_detach(tid);
        }
        else {
          continue;
        }
    }


    while(read(fd1, &pedido, sizeof(Pedido)) > 0) {
        pthread_t tid;
        pthread_create(&tid, NULL, processFifo, (void *) &pedido);
    }

    if(close(fd1) == -1){
        perror("Error closing FIFO");
        exit(1);
    }

    if(unlink(args.fifoname) == -1) {
        perror("ERROR unlinking FIFO");
        exit(1);
    }

    pthread_exit(NULL);
}
