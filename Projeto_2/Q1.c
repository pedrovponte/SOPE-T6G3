#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "args_handler.h"
#include "logs.h"
#include "utils.h"


int current_time = 0;
int max_time = 0;
int place = 0;

void * processFifo(void *req) {

    Pedido pedido = *(Pedido *) req;

    printf("%s\n", "Entered process fifo");

    if(current_time < max_time) {
        place++;
        printf("%s %i\n", "Entered wc and place is ", place);
        current_time += pedido.dur;
        printf("Current time: %i \n", current_time);
        printf("Total time: %i\n", max_time);
        usleep(pedido.dur * 1000000);
    }
    else{
        place = pedido.pl;
        pedido.dur = -1;
        printf("%s\n", "Wc closed");
    }

    Pedido resposta;
    resposta.id = pedido.id;
    resposta.pid = getpid();
    resposta.tid = pthread_self();
    resposta.dur = pedido.dur;
    resposta.pl = place;

    char private_fifo[50];
    sprintf(private_fifo, "/tmp/%d.%d", pedido.pid, pedido.tid);

    int fd2;
    if((fd2 = open(private_fifo, O_RDONLY | O_NONBLOCK)) != 0){
        perror("Error opening fifo.");
        exit(1);
    }

    printf("%s\n", "Openned fifo");

    write(fd2, &resposta, sizeof(Pedido));

    printf("%s\n", "Wrote on fifo");

    if(close(fd2) == -1){
        perror("Error closing fifo.");
        exit(1);
    }

    printf("%s\n", "Closed fifo");

    return 0;
}


int main(int argc, char *argv[]){

    /*if(argc < 4){ 
        perror("Wrong number of arguments.");
        exit(1);
    }*/
    
    int fd1;
    args_q1 args = process_args_q(argc, argv);

    max_time = args.nsecs * 1000000;
    //current_time = 0;
    
    mkfifo(args.fifoname, 0660);

    printf("%s\n", "Created fifo Qn");

    fd1 = open(args.fifoname, O_RDONLY | O_NONBLOCK);

    printf("%s\n", "Opened fifo Qn");

    while(current_time < max_time){
        Pedido pedido;
        while((read(fd1, &pedido, sizeof(Pedido)) != -1) && current_time < max_time){
            printf("%s\n", "Waiting");
            sleep(1);
            current_time += 1000000;
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