#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct {
    unsigned front, rear, size , capacity;
    unsigned *array;
} Queue;

Queue createQueue(unsigned capacity);
int isEmpty(Queue* q);
int isFull(Queue* q);
void eliminate(Queue* q, unsigned client);
int enter(Queue *q);
void fillQueue(Queue *q);
