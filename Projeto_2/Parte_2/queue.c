#include "queue.h"

Queue createQueue(unsigned capacity) {
    Queue q;
    q.capacity = capacity;
    q.front = 0;
    q.size = 0;
    q.rear = capacity - 1;
    q.array = malloc(q.capacity * sizeof(unsigned));

    return q;
}

int isEmpty(Queue* q) {
    return (q->size == 0);
}

int isFull(Queue* q) {
    return (q->size == q->capacity);
}

void eliminate(Queue* q, unsigned client) {
    if(isFull(q))
        return;
    q->rear = (q->rear + 1) % q->capacity;
    q->array[q->rear] = client;
    q->size++;
}

int enter(Queue *q) {
    if(isEmpty(q))
        return 0;
    unsigned item = q->array[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return item;
}

void fillQueue(Queue *q) {
    for(unsigned i = 1; i <= q->capacity; i++) {
        eliminate(q, i);
    }
}
