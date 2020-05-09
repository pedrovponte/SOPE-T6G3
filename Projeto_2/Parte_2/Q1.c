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
    sprintf(private_fifo, "/tmp/%d.%d", pedido.pid, pedido.tid);

    int fd2;

    pedido.pid = getpid();
    pedido.tid = pthread_self();

    if(time(NULL) < max_time) {
        registLog(pedido.id, pedido.pid, pedido.tid, pedido.dur, pedido.pl, "ENTER");
        usleep(pedido.dur);
        registLog(pedido.id, pedido.pid, pedido.tid, pedido.dur, pedido.pl, "TIMUP");
    }
    else{
        pedido.pl = -1;
        pedido.dur = -1;
        registLog(pedido.id, pedido.pid, pedido.tid, pedido.dur, pedido.pl, "2LATE");
        printf("%s\n", "Wc closed");
    }

    int tents = 0;
    do {
      fd2 = open(private_fifo, O_WRONLY);
      tents++;
      if(tents == 20){
          break;
      }
    } while(fd2 == -1);

    if(tents == 20) {
        registLog(pedido.id, pedido.pid, pedido.tid, pedido.dur, pedido.pl, "GAVUP");
    }
    else {
        write(fd2, &pedido, sizeof(Pedido));
    }

    if(close(fd2) == -1){
        perror("Error closing fifo.");
        exit(1);
    }

    return 0;
}


int main(int argc, char *argv[]){

    if(argc < 4){
        perror("Wrong number of arguments.");
        print_usage_q();
        exit(1);
    }

    int fd1, place = 0;
    args_q1 args = process_args_q(argc, argv);

    max_time = time(NULL) + args.nsecs;

    mkfifo(args.fifoname, 0660);

    fd1 = open(args.fifoname, O_RDONLY | O_NONBLOCK);

    Pedido pedido;
    while(time(NULL) < max_time){
        if((read(fd1, &pedido, sizeof(Pedido)) <= 0) && time(NULL) < max_time){
            printf("%s\n", "Waiting for requests...");
            sleep(1);
        }
        if(read(fd1, &pedido, sizeof(Pedido)) > 0) {
          place++;
          pedido.pl = place;
          pthread_t tid;
          pthread_create(&tid, NULL, processFifo, (void *) &pedido);
        }
    }

    sleep(1);
    while(read(fd1, &pedido, sizeof(Pedido)) > 0) {
        pthread_t tid;
        pthread_create(&tid, NULL, processFifo, (void *) &pedido);
        sleep(1);
    }

    if(close(fd1) == -1){
        perror("Error closing fifo.");
        exit(1);
    }

    unlink(args.fifoname);

    return 0;
}
