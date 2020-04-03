#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 512

int main(int argc, char *argv[]){
    int source;
    //unsigned char buffer[BUFFER_SIZE];
    char c;

    if(argc < 2){
        printf("Usage: %s <source> <destination>\n", argv[1]);
        return 1;
    }

    source = open(argv[1], O_RDONLY);
    if(source == 0){
        perror(argv[1]);
        return 2;
    } 

    if(argc == 3){
        int destination = open(argv[2], O_WRONLY);
        dup2(destination, STDOUT_FILENO);
    }
    
    while((read(source,&c,sizeof(char))) != 0){
        //printf("%s",buffer);
        write(STDOUT_FILENO, &c, sizeof(char));
    }
    printf("\n");
    close(source);
    return 0;
}

