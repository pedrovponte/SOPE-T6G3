#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "args_handler.h"
#include "logs.h"
#include "utils.h"

int fd;
char fifo_name[50];
int finish = 0;

void *sendFifo(void * number){

    int fd2;
    Pedido request;

    request.id = *(int *) number;
    request.dur = rand() % 3000001 + 1;
    request.pid = getpid();
    request.tid = pthread_self();
    request.pl = -1;

    /*if(finish == 1) {
      request.dur = -1;
      registLog(request.id, request.pid, request.tid, request.dur, request.pl, "FAILD");
      exit(0);
    }*/

    if(write(fd, &request, sizeof(Pedido)) == -1){
        perror("Error writing to fifo.");
        exit(1);
    }

    registLog(request.id, request.pid, request.tid, request.dur, request.pl, "IWANT");

    char private_fifo[50];
    sprintf(private_fifo, "/tmp/%d.%d", getpid(), request.tid);

    if(mkfifo(private_fifo, 0660) != 0){
        printf("Error creating fifo.\n");
        exit(1);
    }

    if((fd2 = open(private_fifo, O_RDONLY | O_NONBLOCK)) < 0){
        registLog(request.id, request.pid, request.tid, request.dur, request.pl, "FAILD");
        printf("Error opening fifo.\n");
        exit(1);
    }

    Pedido answer;

    while(read(fd2, &answer, sizeof(Pedido)) <= 0){
        usleep(15000);
    }

    if(answer.pl > 0 && answer.dur > 0){
        registLog(answer.id, answer.pid, answer.tid, answer.dur, answer.pl, "IAMIN");
    }
    else{
        registLog(answer.id, answer.pid, answer.tid, answer.dur, answer.pl, "CLOSD");
        finish = 1;
    }

    //escrever logs quando nao entra (diferença entre CLOSD e FAILD, como descobrir?)

    if(close(fd2) == -1){
        perror("Error closing fifo.");
        exit(1);
    }

    unlink(private_fifo);
    return 0;
}

int main(int argc, char *argv[]){

    /*if(argc != 4){
        printf("%s\n", "Wrong number of arguments");
        exit(1);
    }*/

    args_u1 args = process_args_u(argc, argv);

    int id = 0;

    int max_time = time(NULL) + args.nsecs;

    do {
        fd = open(args.fifoname, O_WRONLY);
        if (fd == -1) {
            printf("Connecting to server...\n");
            sleep(1);
        }
    } while(fd == -1);

    while(time(NULL) < max_time){
        int rc;
        pthread_t tid;
        rc = pthread_create(&tid, NULL, sendFifo, (void *) &id);
        if(rc){
            printf("ERROR creating thread: return code from pthread_create() is %d\n", rc);
            exit(1);
        }
        id++;
        sleep(1);
    }

    /*pthread_exit(0);

    if(close(fd) == -1){
        perror("Error closing fifo.");
        exit(1);
    }*/

    return 0;
}
