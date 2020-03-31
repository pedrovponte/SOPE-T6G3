#include <stdio.h>
#include <stdlib.h>
#include "args_handler.h"
#include "dirs.h"
#include "logs.h"


int main(int argc, char *argv[]){
    struct commands coms;

    args_commands(argc, argv, &coms);

    dirs(&coms);

    return 0;
}