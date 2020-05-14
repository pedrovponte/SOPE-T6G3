#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include "args_handler.h"
#include "utils.h"
#include "queue.h"

int max_time = 0, place = 1;
sem_t nPlaces, nThreads;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
Queue q;
int flagPlaces = 0;
int flagThreads = 0;

void * processFifo(void *req) {

    Pedido pedido = *(Pedido *) req;
    registLog(pedido.id, pedido.pid, pedido.tid, pedido.dur, pedido.pl, "RECVD");


    char private_fifo[50];
    sprintf(private_fifo, "/tmp/%d.%ld", pedido.pid, pedido.tid);

    int fd2;
    if((fd2 = open(private_fifo, O_WRONLY)) == -1) {
        perror("ERROR opening private FIFO");
        exit(1);
    }

    pedido.pid = getpid();
    pedido.tid = pthread_self();

    int client_place;

    if(time(NULL) < max_time) {
        if(flagPlaces) {
            if(sem_wait(&nPlaces) != 0) {
                perror("ERROR on semaphore wait");
            }
            if(pthread_mutex_lock(&mutex) != 0) {
                perror("ERROR locking the mutex");
            }
            client_place = enter(&q);
            if(pthread_mutex_unlock(&mutex) != 0) {
                perror("ERROR unlocking the mutex");
            }
        }
        else {
            if(pthread_mutex_lock(&mutex) != 0) {
                perror("ERROR locking the mutex");
            }
            client_place = place;
            place++;
            if(pthread_mutex_unlock(&mutex) != 0) {
                perror("ERROR unlocking the mutex");
            }
        }
        pedido.pl = client_place;
        //pedido.pid = getpid();
        //pedido.tid = pthread_self();
        registLog(pedido.id, pedido.pid, pedido.tid, pedido.dur, pedido.pl, "ENTER");
    }
    else{
        pedido.pl = -1;
        pedido.dur = -1;
        //pedido.pid = getpid();
        //pedido.tid = pthread_self();
        registLog(pedido.id, pedido.pid, pedido.tid, pedido.dur, pedido.pl, "2LATE");
    }

    int i = write(fd2, &pedido, sizeof(Pedido));


    if(i < 0) {
        perror("ERROR writing to private FIFO");
        registLog(pedido.id, pedido.pid, pedido.tid, pedido.dur, pedido.pl, "GAVUP");
        if(close(fd2) < 0) {
            perror("ERROR closing private FIFO");
        }
        if (flagPlaces) {
            if(pthread_mutex_lock(&mutex) != 0) {
                perror("ERROR locking the mutex");
            }
            eliminate(&q, client_place);
            if(pthread_mutex_unlock(&mutex) != 0) {
                perror("ERROR unlocking the mutex");
            }
            if(sem_post(&nPlaces) != 0) {
                perror("ERROR on semaphore post");
            }
        }
        if(flagThreads) {
            sem_post(&nThreads);
        }
        pthread_exit(NULL);
    }

    if(close(fd2)){
        perror("ERROR closing private FIFO");
        exit(1);
    }

    if(time(NULL) < max_time) {
        usleep(pedido.dur * 1000);
        registLog(pedido.id, pedido.pid, pedido.tid, pedido.dur, pedido.pl, "TIMUP");
    }

    if (flagPlaces) {
        if(pthread_mutex_lock(&mutex) != 0) {
            perror("ERROR locking the mutex");
        }
        eliminate(&q, client_place);
        if(pthread_mutex_unlock(&mutex) != 0) {
            perror("ERROR unlocking the mutex");
        }
        if(sem_post(&nPlaces) != 0) {
            perror("ERROR on semaphore post");
        }
    }

    if(flagThreads) {
        sem_post(&nThreads);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]){

    if(argc < 4){
        perror("Wrong number of arguments");
        print_usage_q();
        exit(1);
    }

    int fd1, i = 0;
    args_q1 args = process_args_q(argc, argv);
    printf("%i\n", args.nthreads);

    if(args.nplaces < __INT_MAX__)
        flagPlaces = 1;

    if(args.nthreads < __INT_MAX__)
        flagThreads = 1;

    if(flagPlaces) {
        sem_init(&nPlaces, 0, args.nplaces);
        q = createQueue(args.nplaces);
        fillQueue(&q);
    }

    if(flagThreads) {
        sem_init(&nThreads, 0, args.nthreads);
    }

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
            if(flagThreads) {
                sem_wait(&nThreads);
            }
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
        if(flagThreads) {
            sem_wait(&nThreads);
        }
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
