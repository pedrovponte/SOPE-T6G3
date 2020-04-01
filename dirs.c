#include "dirs.h"

#define READ 0
#define WRITE 1

int dirs(struct commands *coms, const char *path_file){
    long int total = 0;
    long int total_rest = 0;
    struct stat path_stat;
    struct dirent *de;
    int fd[2];
    int pid;

    DIR *dir = opendir(path_file);

    if(dir == NULL){
        return 0;
    }

    while((de = readdir(dir)) != NULL){

        if(strcmp(de->d_name,".") == 0 || strcmp(de->d_name,"..") == 0){
            
            continue;
        }
        
        lstat(de->d_name, &path_stat);
        
        if(S_ISREG(path_stat.st_mode)){    
            if(coms->all_files == 1){
                if(coms->show_bytes == 1 && coms->block_size == 1){     //faltam bytes
                    if((path_stat.st_size % coms->block_size_bytes) == 0){
                        printf("%li", (long int)(path_stat.st_size / coms->block_size_bytes));
                        printf("    /");
                        printf("%s\n", de->d_name);
                        total += path_stat.st_size / coms->block_size_bytes;
                    }
                    else{
                        printf("%li", (long int)(path_stat.st_size / coms->block_size_bytes + 1));
                        printf("    /");
                        printf("%s\n", de->d_name);
                        total += path_stat.st_size / coms->block_size_bytes + 1;
                    }
                }
                if(coms->block_size == 1 && coms->show_bytes == 0){             //faltam bytes
                    if(((path_stat.st_blocks / 2) * 1024 % coms->block_size_bytes) == 0) {
                        printf("%li", (long int)((path_stat.st_blocks / 2) * 1024 / coms->block_size_bytes));
                        printf("    /");
                        printf("%s\n", de->d_name);
                        total += (path_stat.st_blocks / 2) * 1024 / coms->block_size_bytes;
                    }
                    else{
                        printf("%li", (long int)((path_stat.st_blocks / 2) * 1024 / coms->block_size_bytes + 1));
                        printf("    /");
                        printf("%s\n", de->d_name);
                        total += (path_stat.st_blocks / 2) * 1024 / coms->block_size_bytes + 1;
                    }
                }
                if(coms->block_size == 0 && coms->show_bytes == 1){
                    printf("%ld", path_stat.st_size);
                    printf("    /");
                    printf("%s\n", de->d_name);
                    total += path_stat.st_size;             //faltam 4096 bytes
                }
                if(coms->block_size == 0 && coms->show_bytes == 0){
                    printf("%ld", path_stat.st_blocks / 2);
                    printf("    /");
                    printf("%s\n", de->d_name);
                    total += path_stat.st_blocks / 2;     //faltam 4 blocos
                }
            }
            else{
                if(coms->show_bytes == 1 && coms->block_size == 1){
                    if((path_stat.st_size % coms->block_size_bytes) == 0){
                        total += path_stat.st_size / coms->block_size_bytes;
                    }
                    else{
                        total += path_stat.st_size / coms->block_size_bytes + 1;
                    }
                }
                if(coms->block_size == 1 && coms->show_bytes == 0){               //faltam 2048 bytes
                    if(((path_stat.st_blocks / 2) * 1024 % coms->block_size_bytes) == 0) {
                        total += (path_stat.st_blocks / 2) * 1024 / coms->block_size_bytes;
                    }
                    else{
                        total += (path_stat.st_blocks / 2) * 1024 / coms->block_size_bytes + 1;
                    }
                }
                if(coms->block_size == 0 && coms->show_bytes == 1){
                    total += path_stat.st_size;
                }
                if(coms->block_size == 0 && coms->show_bytes == 0){
                    total += path_stat.st_blocks / 2;     //faltam 4 blocos
                }
            }
        }

        if(coms->dereference == 1){                  //Corrigir
            if(S_ISLNK(path_stat.st_mode)){                
                total += path_stat.st_blocks / 2;
            }
        }


        if(S_ISDIR(path_stat.st_mode)){   
            pipe(fd);

            pid = fork();

            if(pid == 0){
                if(chdir(de->d_name) != 0){
                    exit(1);
                }
                

                dir = opendir(".");
                total = path_stat.st_blocks / 2;
                if(dir == NULL){
                    exit(0);
                }

                /*close(fd[READ]);
                write(fd[WRITE],&total,8);*/

                //printf("Total filho: %li\n",total);

            }

            else if(pid > 0){
                
                wait(NULL);

                /*close(fd[WRITE]);
                read(fd[READ],&total_rest,8);

                total = total + total_rest;*/

            }

            else{
                fprintf(stderr, "fork error\n");
                return 1;
            }
        }
    }
    
    if(pid == 0){
        close(fd[READ]);
        write(fd[WRITE],&total,8);
         
    }
    else{
        close(fd[WRITE]);
        read(fd[READ],&total_rest,8);
        total += total_rest;
        //printf("Total rest: %li\n",total_rest);
    }
    
    /*if(coms->show_bytes == 0){
        total += 4;
    }*/

    printf("%ld\n", total);
    
    
    closedir(dir);
    return 0;

}
