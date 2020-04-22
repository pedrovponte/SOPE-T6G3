#include "utils.h"

char *construct_fifo_name(int pid, int tid){
    char name[40];
    strcat(name, "/tmp/");
    strcat(name, pid);
    strcat(name, ".");
    strcat(name, tid);

    return name;
}