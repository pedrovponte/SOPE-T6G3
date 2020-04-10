#include "utils.h"


bool check_is_dir(const char* path){
    struct stat path_stat;

    if(stat(path, &path_stat) != 0){
        char info[100];
        strcat(info, path);
        fprintf(stderr, "Invalid argument: %s\n", info);
        exitLog(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    return S_ISDIR(path_stat.st_mode);
}

int print_screen(const char* path, long int size){
    char *dir;
    dir = malloc(2048 * sizeof(char));
    sprintf(dir, "%ld\t%s\n", size, path);
    write(STDOUT_FILENO, dir, strlen(dir));
    entryLog(dir);
    free(dir);
    return 0;
}
