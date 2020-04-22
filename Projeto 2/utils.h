#include <string.h>
#include <stdlib.h>

typedef struct {
    int i;
    int pid;
    int tid;
    int dur;
    int pl;
} pedido;

char *construct_fifo_name(int pid, int tid);