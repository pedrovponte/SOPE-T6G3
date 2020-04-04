#include "dirs.h"

#define READ 0
#define WRITE 1

extern char *directory;

int dirs(struct commands *coms, char *path_file){
    long int total = 0;
    long int total_rest = 0;
    long int size = 0;
    struct stat path_stat;
    struct dirent *de;
    int fd[2];
    int pid;
    char *name, *dir_name, *subdir;

    DIR *dir = opendir(path_file);

    if(dir == NULL){
        perror("Error opening dir");
        exit(1);
    }

    while((de = readdir(dir)) != NULL){
        dir_name = de->d_name;
        //printf("%s\n",de->d_name);

        if(!strcmp(dir_name,".") || !strcmp(dir_name,"..")){
            continue;
        }

        if(!strcmp(".", path_file) || !strcmp("..", path_file)){
            return 0;
        }

        name = (char*) malloc (512 * sizeof(char));

        sprintf(name, "%s%s", path_file, dir_name);
        
        if(stat(name, &path_stat) == -1){
            perror("Stat error");
            exit(1);
        }           
        
        if(S_ISREG(path_stat.st_mode)){    
            char *size_file;

            if(coms->all_files == 1){
                if(coms->show_bytes == 1 && coms->block_size == 1){     //faltam bytes
                    if((path_stat.st_size % coms->block_size_bytes) == 0){
                        size = path_stat.st_size / coms->block_size_bytes;
                        size_file = (char*) malloc(512 * 2 * sizeof(char));
                        sprintf(size_file, "%ld\t%s\n", size, name);
                        write(STDOUT_FILENO, size_file, strlen(size_file));
                        free(size_file);
                        total += size;
                    }
                    else{
                        size = path_stat.st_size / coms->block_size_bytes + 1;
                        size_file = (char*) malloc(512 * 2 * sizeof(char));
                        sprintf(size_file, "%ld\t%s\n", size, name);
                        write(STDOUT_FILENO, size_file, strlen(size_file));
                        free(size_file);
                        total += size;
                    }
                }
                if(coms->block_size == 1 && coms->show_bytes == 0){             //faltam bytes
                    if(((path_stat.st_blocks / 2) * 1024 % coms->block_size_bytes) == 0) {
                        size = (path_stat.st_blocks / 2) * 1024 / coms->block_size_bytes;
                        size_file = (char*) malloc(512 * 2 * sizeof(char));
                        sprintf(size_file, "%ld\t%s\n", size, name);
                        write(STDOUT_FILENO, size_file, strlen(size_file));
                        free(size_file);
                        total += size;
                    }
                    else{
                        size = (path_stat.st_blocks / 2) * 1024 / coms->block_size_bytes + 1;
                        size_file = (char*) malloc(512 * 2 * sizeof(char));
                        sprintf(size_file, "%ld\t%s\n", size, name);
                        write(STDOUT_FILENO, size_file, strlen(size_file));
                        free(size_file);
                        total += size;
                    }
                }
                if(coms->block_size == 0 && coms->show_bytes == 1){
                    size = path_stat.st_size;
                    size_file = (char*) malloc(512 * 2 * sizeof(char));
                    sprintf(size_file, "%ld\t%s\n", size, name);
                    write(STDOUT_FILENO, size_file, strlen(size_file));
                    free(size_file);
                    total += size;           //faltam 4096 bytes
                }
                if(coms->block_size == 0 && coms->show_bytes == 0){
                    size = path_stat.st_blocks / 2;
                    size_file = (char*) malloc(512 * 2 * sizeof(char));
                    sprintf(size_file, "%ld\t%s\n", size, name);
                    write(STDOUT_FILENO, size_file, strlen(size_file));
                    free(size_file);
                    total += size;    //faltam 4 blocos
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
            subdir = (char*) malloc(strlen(path_file) + strlen(dir_name) + 2);
            sprintf(subdir, "%s%s/", path_file, dir_name);

            pipe(fd);

            pid = fork();

            if(pid > 0){
                
                wait(NULL);

                close(fd[WRITE]);
                if(read(fd[READ],&total_rest,8) == -1){
                    perror("Error reading from pipe");
                    exit(1);
                }

                total += total_rest;

            }

            else if(pid == 0){
                signal(SIGINT, SIG_IGN);
                
                dirs(coms, subdir);
                //closedir(dir);

                //close(fd[READ]);
                //write(fd[WRITE],&total,8);
            }

            else{
                fprintf(stderr, "fork error\n");
                return 1;
            }
            free(subdir);
        }
    }
    
    if(pid == 0){
        close(fd[READ]);
        if(write(fd[WRITE],&total,8) == -1){
            perror("Error writing to pipe");
            exit(1);
        }
    }
    /*else{
        close(fd[WRITE]);
        read(fd[READ],&total_rest,8);
        total += total_rest;
        //printf("Total rest: %li\n",total_rest);
    }*/

    total += 4;
    if(strcmp(path_file, directory)){
        if(!strcmp(".", directory)){
            path_file = ".";
        }
        else{
            path_file[strlen(path_file) - 1] = '\0';
        }
    }
    
    char *final_dir;
    final_dir = (char*) malloc(512 * 2 * sizeof(char));
    sprintf(final_dir, "%ld\t%s\n", total, path_file);
    write(STDOUT_FILENO, final_dir, strlen(final_dir));
    free(final_dir);
    free(name);
    
    closedir(dir);
    return 0;
}
