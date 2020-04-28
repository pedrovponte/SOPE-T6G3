#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "args_handler.h"
#include "logs.h"
#include "utils.h"


int max_time = 0;
int place = 0;

void * processFifo(void *req) {

    Pedido pedido = *(Pedido *) req;
    registLog(pedido.id, pedido.pid, pedido.tid, pedido.dur, pedido.pl, "RECVD");

    char private_fifo[50];
    sprintf(private_fifo, "/tmp/%d.%d", pedido.pid, pedido.tid);

    int fd2;

    Pedido resposta;
    resposta.id = pedido.id;
    resposta.pid = getpid();
    resposta.tid = pthread_self();
    resposta.dur = pedido.dur;

    do {
      fd2 = open(private_fifo, O_WRONLY);
    } while(fd2 == -1);

    if(time(NULL) < max_time) {
        place++;
        registLog(pedido.id, pedido.pid, pedido.tid, pedido.dur, place, "ENTER");
        resposta.pl = place;
        write(fd2, &resposta, sizeof(Pedido));
        usleep(pedido.dur);
    }
    else{
        resposta.pl = -1;
        resposta.dur = -1;
        registLog(pedido.id, pedido.pid, pedido.tid, pedido.dur, place, "2LATE");
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

    int fd1;
    args_q1 args = process_args_q(argc, argv);

    max_time = time(NULL) + args.nsecs;

    mkfifo(args.fifoname, 0660);

    printf("%s\n", "Created fifo Qn");

    fd1 = open(args.fifoname, O_RDONLY | O_NONBLOCK);

    printf("%s\n", "Opened fifo Qn");

    while(/*current_time*/ time(NULL) < max_time){
        Pedido pedido;
        while((read(fd1, &pedido, sizeof(Pedido)) <= 0) && /*current_time*/ time(NULL) < max_time){
            printf("%s\n", "Waiting");
            sleep(1);
        }
        pthread_t tid;
        pthread_create(&tid, NULL, processFifo, (void *) &pedido);
        printf("%s\n", "Created fifo Qn");
    }

    pthread_exit(0);

    if(close(fd1) == -1){
        perror("Error closing fifo.");
        exit(1);
    }

    unlink(args.fifoname);

    return 0;
}
