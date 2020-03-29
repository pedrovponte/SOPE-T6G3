#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "main.c"
//#include <limits.h>

#define READ 0
#define WRITE 1

extern struct commands coms;

int main(void){
    
    long int total = 4;
    //long int total_rest = 0;
    struct stat path_stat;
    struct dirent *de;
    //int fd[2];
    //int pid;
    //int aux = -1;

    DIR *dir = opendir(".");

    if(dir == NULL){
        return 0;
    }

    while((de = readdir(dir)) != NULL){
        
        if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name,"..") == 0){
            
            continue;
        }
        
        lstat(de->d_name, &path_stat);
        
        if(S_ISREG(path_stat.st_mode)){    
            total += path_stat.st_blocks/2;
        }
	
        if(coms.all_files == 1){
            printf("%ld", path_stat.st_blocks / 2);
            printf("    %s\n", de->d_name);
        }

        if(coms.dereference == 1){
            if(S_ISLNK(path_stat.st_mode)){
                total += path_stat.st_blocks / 2;
            }
            printf("%ld", path_stat.st_blocks / 2);
            printf("    %s\n", de->d_name);
        }

        /*if(S_ISDIR(path_stat.st_mode)){
                
            //pipe(fd);

            pid = fork();

            if(pid == 0){
                if(chdir(de->d_name) != 0){
                    exit(1);
                }
                

                dir = opendir(".");
                total = 4;
                if(dir == NULL){
                    exit(0);
                }

                close(fd[READ]);
                write(fd[WRITE],&total,8);

            }

            else if(pid > 0){
                parent = getpid();
                wait(NULL);

                close(fd[WRITE]);
                read(fd[READ],&total_rest,8);

                total = total + total_rest;
            }

            else{
                fprintf(stderr, "fork error\n");
                return 1;
            }
        }*/
    }
    
    /*if(pid == 0){
        close(fd[READ]);
        
        write(fd[WRITE],&total,8);
         
    }
    else{
        close(fd[WRITE]);
        read(fd[READ],&total_rest,8);
        total = total + total_rest;
    }*/
    
    printf("%ld\n", total);
    /*printf("%s", de->d_name);*/
    
    closedir(dir);
    return 0;

}
