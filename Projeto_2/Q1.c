#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "args_handler.h"
#include "logs.h"
#include "utils.h"


int max_time = 0;
int start = 0;

void * processFifo(void *req) {

    Pedido pedido = *(Pedido *) req;
    if(start == 1)
        registLog(pedido.id, pedido.pid, pedido.tid, pedido.dur, pedido.pl, "RECVD");

    char private_fifo[50];
    sprintf(private_fifo, "/tmp/%d.%d", pedido.pid, pedido.tid);

    int fd2;

    Pedido resposta;
    resposta.id = pedido.id;
    resposta.pid = getpid();
    resposta.tid = pthread_self();
    resposta.pl = pedido.pl;
    resposta.dur = pedido.dur;

    do {
      fd2 = open(private_fifo, O_WRONLY);
    } while(fd2 == -1);

    if(time(NULL) < max_time) {
        registLog(pedido.id, pedido.pid, pedido.tid, pedido.dur, pedido.pl, "ENTER");
        write(fd2, &resposta, sizeof(Pedido));
        usleep(pedido.dur);
        registLog(pedido.id, pedido.pid, pedido.tid, pedido.dur, pedido.pl, "TIMUP");
    }
    else{
        resposta.pl = -1;
        resposta.dur = -1;
        registLog(pedido.id, pedido.pid, pedido.tid, pedido.dur, pedido.pl, "2LATE");
        write(fd2, &resposta, sizeof(Pedido));
        printf("%s\n", "Wc closed");
    }

    if(close(fd2) == -1){
        perror("Error closing fifo.");
        exit(1);
    }

    return 0;
}


int main(int argc, char *argv[]){

    /*if(argc < 4){
        perror("Wrong number of arguments.");
        exit(1);
    }*/

    int fd1, place = -1;
    args_q1 args = process_args_q(argc, argv);

    max_time = time(NULL) + args.nsecs;

    mkfifo(args.fifoname, 0660);

    fd1 = open(args.fifoname, O_RDONLY | O_NONBLOCK);

    Pedido pedido;
    //read(fd1, &pedido, sizeof(Pedido));
    while(time(NULL) < max_time){
        //Pedido pedido;
        /*while((read(fd1, &pedido, sizeof(Pedido)) <= 0) && time(NULL) < max_time){
            printf("%s\n", "Waiting for requests");
            sleep(1);
        }*/
        place++;
        pedido.pl = place;
        pthread_t tid;
        pthread_create(&tid, NULL, processFifo, (void *) &pedido);
        while((read(fd1, &pedido, sizeof(Pedido)) <= 0) && time(NULL) < max_time){
            printf("%s\n", "Waiting for requests");
            sleep(1);
        }
        start = 1;
    }

    sleep(1);
    //Pedido pedido;
    while(read(fd1, &pedido, sizeof(Pedido)) > 0) {
        pthread_t tid;
        pthread_create(&tid, NULL, processFifo, (void *) &pedido);
        sleep(1);
    }

    //pthread_exit(0);

    if(close(fd1) == -1){
        perror("Error closing fifo.");
        exit(1);
    }

    unlink(args.fifoname);

    return 0;
}
