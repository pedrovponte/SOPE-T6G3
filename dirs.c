#include "dirs.h"

#define READ 0
#define WRITE 1

extern char *directory;
extern int fd[2];
extern commands coms;
extern pid_t pgid;
pid_t son;

int dirs(char *path_file, int level){
    
    long int total = 0;
    struct stat path_stat;
    struct dirent *de;
    pid_t pid;
    int status;
    char *name, *dir_name, *subdir/*, *d*/;

    DIR *dir = opendir(path_file);

    if(dir == NULL){
        perror("Error opening dir");
        exitLog(EXIT_FAILURE);
        exit(1);
    }

    while((de = readdir(dir)) != NULL){
        dir_name = de->d_name;
        //printf("%s\n",de->d_name);

        if(strcmp(dir_name,".") == 0 || strcmp(dir_name,"..") == 0){
            continue;
        }

        if(strcmp(".", path_file) == 0 || strcmp("..", path_file) == 0){
            return 0;
        }

        name = malloc(225 * sizeof(char));

        sprintf(name, "%s%s", path_file, dir_name);
        
        if(coms.dereference == 1){
            //printf("Lstat\n");
            if(stat(name, &path_stat) == -1){
                perror("Stat error");
                exitLog(EXIT_FAILURE);
                exit(1);
            }
        }
        else{
            if(lstat(name, &path_stat) == -1){
                perror("Lstat error");
                exitLog(EXIT_FAILURE);
                exit(1);
            }
        }    

        /*if(S_ISLNK(path_stat.st_mode)){
            printf("Tamanho: %li\n", path_stat.st_blocks / 2);
        }*/  
        
        if(S_ISREG(path_stat.st_mode) || S_ISLNK(path_stat.st_mode)){    
            long int size;
            //char *size_file;

            if(coms.all_files == 1 && level < coms.max_depth_size){
                if(coms.show_bytes == 1 && coms.block_size == 1){     
                    if((path_stat.st_size % coms.block_size_bytes) == 0){
                        size = path_stat.st_size / coms.block_size_bytes;
                    }
                    else{
                        size = path_stat.st_size / coms.block_size_bytes + 1;
                    }
                }
                if(coms.block_size == 1 && coms.show_bytes == 0){             
                    if(((path_stat.st_blocks / 2) * 1024 % coms.block_size_bytes) == 0) {
                        size = (path_stat.st_blocks / 2) * 1024 / coms.block_size_bytes;
                    }
                    else{
                        size = (path_stat.st_blocks / 2) * 1024 / coms.block_size_bytes + 1;
                    }
                }
                if(coms.block_size == 0 && coms.show_bytes == 1){
                    size = path_stat.st_size;   
                }
                if(coms.block_size == 0 && coms.show_bytes == 0){
                    size = path_stat.st_blocks / 2; 
                }
                /*size_file = malloc(2048 * sizeof(char));
                sprintf(size_file, "%ld\t%s\n", size, name);
                write(STDOUT_FILENO, size_file, strlen(size_file));
                entryLog(size_file);
                free(size_file);*/
                if(print_screen(name, size) != 0){
                    perror("Error writing on the screen\n");
                    exitLog(EXIT_FAILURE);
                    exit(1);
                }
                total += size;
            }
            else{
                if(coms.show_bytes == 1 && coms.block_size == 1){
                    if((path_stat.st_size % coms.block_size_bytes) == 0){
                        total += path_stat.st_size / coms.block_size_bytes;
                    }
                    else{
                        total += path_stat.st_size / coms.block_size_bytes + 1;
                    }
                }
                if(coms.block_size == 1 && coms.show_bytes == 0){               
                    if(((path_stat.st_blocks / 2) * 1024 % coms.block_size_bytes) == 0) {
                        total += (path_stat.st_blocks / 2) * 1024 / coms.block_size_bytes;
                    }
                    else{
                        total += (path_stat.st_blocks / 2) * 1024 / coms.block_size_bytes + 1;
                    }
                }
                if(coms.block_size == 0 && coms.show_bytes == 1){
                    total += path_stat.st_size;
                }
                if(coms.block_size == 0 && coms.show_bytes == 0){
                    total += path_stat.st_blocks / 2;     
                }
            }
        }

        if(S_ISDIR(path_stat.st_mode)){   
            subdir = malloc(2048);
            sprintf(subdir, "%s%s/", path_file, dir_name);

            pid = fork();

            if(pid > 0){
                
                if(getpgrp() == pgid){
                    son = pid;
                }
                
                waitpid(pid, &status, 0);
                long int total_rest;

                if(!coms.separate_dirs){
                    if(read(fd[READ],&total_rest,sizeof(long int)) == -1){
                        perror("Error reading from pipe");
                        exitLog(EXIT_FAILURE);
                        exit(1);
                    }
                    total += total_rest;
                }
                recvPipeLog(total_rest);
                //printf("Ola do pai\n");
                //printf("Total Rest: %li\n", total_rest);
                //total += total_rest;
                //printf("Total: %li\n", total);
                //exitLog(EXIT_SUCCESS);
                //exit(0);
            }

            else if(pid == 0){
                //signal(SIGINT, SIG_IGN);
                
                if(getpgrp() == pgid){
                    setpgid(pid, getpid());
                }

                dirs(subdir, level + 1);

                //printf("Ola do filho %i\n",getpid());
                
                if(closedir(dir) == -1){
                    perror("Error closing dir");
                    exitLog(EXIT_FAILURE);
                    exit(1);
                }
                exitLog(EXIT_SUCCESS);
                exit(0);

                //close(fd[READ]);
                //write(fd[WRITE],&total,sizeof(long int));
            }

            else{
                fprintf(stderr, "fork error\n");
                exitLog(EXIT_FAILURE);
                return 1;
            }
            if(!coms.separate_dirs){
                if(coms.show_bytes == 1){
                    total += path_stat.st_size;
                }
                else{
                    total += (path_stat.st_blocks / 2) * 1024 / coms.block_size_bytes;
                }
            }
            free(subdir);
        }
    }
    
    //if(pid == 0){
        //close(fd[READ]);
    if(!coms.separate_dirs){
        if(write(fd[WRITE],&total,sizeof(long int)) == -1){
            perror("Error writing to pipe");
            exitLog(EXIT_FAILURE);
            exit(1);
        }
        sendPipeLog(total);
    }
        //printf("Filho %i escreveu: %li\n",getpid(),total);
    //}
    
    /*else{
        close(fd[WRITE]);
        read(fd[READ],&total_rest,8);
        total += total_rest;
        //printf("Total rest: %li\n",total_rest);
    }*/

    //total += 4;

    if(coms.show_bytes == 1){
        total += 4096;
    }
    else{
        total += 4096 / coms.block_size_bytes;
    }

    /*d = path_file;
    d[strlen(d) - 1] = '\0';*/

    if(level <= coms.max_depth_size){
        if(strcmp(path_file, directory)){       //tirar a barra do final do diretorio
            if(!strcmp(".", directory)){
                path_file = ".";
            }
            else{
                path_file[strlen(path_file) - 1] = '\0';
            }
        }
        /*char *final_dir;
        final_dir = malloc(2048 * sizeof(char));
        sprintf(final_dir, "%ld\t%s\n", total, path_file);
        write(STDOUT_FILENO, final_dir, strlen(final_dir));
        entryLog(final_dir);
        free(final_dir);*/
        if(print_screen(path_file, total) != 0){
            perror("Error writing on the screen\n");
            exitLog(EXIT_FAILURE);
            exit(1);
        }
    }
    
    //free(name);
    
    //closedir(dir);
    
    return 0;
}
