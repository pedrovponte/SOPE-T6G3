#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 512

int main(int argc, char *argv[]){
    FILE *fd1;
    unsigned char buffer[BUFFER_SIZE];

    if(argc != 2){
        printf("Usage: %s <source> <destination>\n", argv[1]);
        return 1;
    }
    fd1 = fopen(argv[1], "r");
    if(fd1 == NULL){
        perror(argv[1]);
        return 2;
    } 
    while((fread(buffer, sizeof(char), 1, fd1)) > 0){
        printf("%s",buffer);
    }
    printf("\n");
    fclose(fd1);
    return 0;
}