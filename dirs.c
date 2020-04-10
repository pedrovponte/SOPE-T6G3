#include "dirs.h"

#define READ 0
#define WRITE 1

extern bool inp_dir;
extern char *act_dir;
extern int fd[2];
extern commands coms;
extern pid_t pgid;
pid_t son;

int dirs(char *path_file, int level)
{

    long int total = 0;
    struct stat path_stat;
    struct dirent *de;
    pid_t pid;
    int status;
    char *name, *dir_name, *subdir, *d;
    DIR *dir = opendir(path_file);

    if (dir == NULL){
        perror("Error opening dir");
        exitLog(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    while ((de = readdir(dir)) != NULL){
        
        dir_name = de->d_name;
        
        if (strcmp(dir_name, ".") == 0 || strcmp(dir_name, "..") == 0)
        {
            continue;
        }

        if (strcmp(".", path_file) == 0 || strcmp("..", path_file) == 0)
        {
            return 0;
        }

        name = malloc(225 * sizeof(char));

        sprintf(name, "%s%s", path_file, dir_name);

        if (coms.dereference == 1)
        {
            if (stat(name, &path_stat) == -1)
            {
                perror("Stat error");
                exitLog(EXIT_FAILURE);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            if (lstat(name, &path_stat) == -1)
            {
                perror("Lstat error");
                exitLog(EXIT_FAILURE);
                exit(EXIT_FAILURE);
            }
        }

        if (S_ISREG(path_stat.st_mode) || S_ISLNK(path_stat.st_mode))
        {
            long int size;

            if (coms.all_files == 1 && level < coms.max_depth_size)
            {
                if (coms.show_bytes == 1 && coms.block_size == 1)
                {
                    if ((path_stat.st_size % coms.block_size_bytes) == 0)
                    {
                        size = path_stat.st_size / coms.block_size_bytes;
                    }
                    else
                    {
                        size = path_stat.st_size / coms.block_size_bytes + 1;
                    }
                }
                if (coms.block_size == 1 && coms.show_bytes == 0)
                {
                    if (((path_stat.st_blocks / 2) * 1024 % coms.block_size_bytes) == 0)
                    {
                        size = (path_stat.st_blocks / 2) * 1024 / coms.block_size_bytes;
                    }
                    else
                    {
                        size = (path_stat.st_blocks / 2) * 1024 / coms.block_size_bytes + 1;
                    }
                }
                if (coms.block_size == 0 && coms.show_bytes == 1)
                {
                    size = path_stat.st_size;
                }
                if (coms.block_size == 0 && coms.show_bytes == 0)
                {
                    size = path_stat.st_blocks / 2;
                }
    
                if (print_screen(name, size) != 0)
                {
                    perror("Error writing on the screen\n");
                    exitLog(EXIT_FAILURE);
                    exit(EXIT_FAILURE);
                }
                total += size;
            }
            else
            {
                if (coms.show_bytes == 1 && coms.block_size == 1)
                {
                    if ((path_stat.st_size % coms.block_size_bytes) == 0)
                    {
                        total += path_stat.st_size / coms.block_size_bytes;
                    }
                    else
                    {
                        total += path_stat.st_size / coms.block_size_bytes + 1;
                    }
                }
                if (coms.block_size == 1 && coms.show_bytes == 0)
                {
                    if (((path_stat.st_blocks / 2) * 1024 % coms.block_size_bytes) == 0)
                    {
                        total += (path_stat.st_blocks / 2) * 1024 / coms.block_size_bytes;
                    }
                    else
                    {
                        total += (path_stat.st_blocks / 2) * 1024 / coms.block_size_bytes + 1;
                    }
                }
                if (coms.block_size == 0 && coms.show_bytes == 1)
                {
                    total += path_stat.st_size;
                }
                if (coms.block_size == 0 && coms.show_bytes == 0)
                {
                    total += path_stat.st_blocks / 2;
                }
            }
        }

        if (S_ISDIR(path_stat.st_mode))
        {
            subdir = malloc(2048);
            sprintf(subdir, "%s%s/", path_file, dir_name);

            pid = fork();

            if (pid > 0)
            {

                if (getpgrp() == pgid)
                {
                    son = pid;
                }

                waitpid(pid, &status, 0);
                long int total_rest;

                if (!coms.separate_dirs)
                {
                    if (read(fd[READ], &total_rest, sizeof(long int)) == -1)
                    {
                        perror("Error reading from pipe");
                        exitLog(EXIT_FAILURE);
                        exit(EXIT_FAILURE);
                    }
                    total += total_rest;
                }
                recvPipeLog(total_rest);
            }

            else if (pid == 0)
            {

                if (getpgrp() == pgid)
                {
                    setpgid(pid, getpid());
                }

                dirs(subdir, level + 1);

                if (closedir(dir) == -1)
                {
                    perror("Error closing dir");
                    exitLog(EXIT_FAILURE);
                    exit(1);
                }
                exitLog(EXIT_SUCCESS);
                exit(EXIT_SUCCESS);
            }

            else
            {
                fprintf(stderr, "fork error\n");
                exitLog(EXIT_FAILURE);
                return 1;
            }
            if (!coms.separate_dirs)
            {
                if (coms.show_bytes == 1)
                {
                    total += path_stat.st_size;
                }
                else
                {
                    total += (path_stat.st_blocks / 2) * 1024 / coms.block_size_bytes;
                }
            }
            free(subdir);
        }
    }

    if (!coms.separate_dirs)
    {
        if (write(fd[WRITE], &total, sizeof(long int)) == -1)
        {
            perror("Error writing to pipe");
            exitLog(EXIT_FAILURE);
            exit(EXIT_FAILURE);
        }
        sendPipeLog(total);
    };

    stat(path_file,&path_stat);

    if (coms.show_bytes == 1)
    {
        total += path_stat.st_size;

        //total += 4096;
    }
    else
    {
        
        total += path_stat.st_size / coms.block_size_bytes;
        //total += 4096 / coms.block_size_bytes;
    }
    if (!inp_dir)
    {
        d = path_file;
        if(d[strlen(d) - 1] == '/'){
            d[strlen(d) - 1] = '\0';
        }
    }
    if (level <= coms.max_depth_size)
    {
        if (strcmp(path_file, act_dir))
        {                                           //tirar a barra do final do diretorio
            if (!strcmp(".", act_dir))
            {
                path_file = ".";
            }
            else
            {
                //path_file[strlen(path_file) - 1] = '\0';
            }
        }
        if (inp_dir)
        {
            if (print_screen(path_file, total) != 0)
            {
                perror("Error writing on the screen\n");
                exitLog(EXIT_FAILURE);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            if (print_screen(d, total) != 0)
            {
                perror("Error writing on the screen\n");
                exitLog(EXIT_FAILURE);
                exit(EXIT_FAILURE);
            }
        }
    }
    return 0;
}