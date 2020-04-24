#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include "logs.h"

void sigint_handler();
void sigterm_handler();
void sigcont_handler();
int init_signals();