#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define SIZE 34

void child(int* parent_pipe)
{
    close(parent_pipe[1]);
    int child_pipe[2];
    int cur;
    int res = read(parent_pipe[0], &cur, sizeof(int)); //从父进程读出的数暂存在cur上
    if(res == 0){
        exit(0);
    }
    pipe(child_pipe);
    if(fork() == 0){
        child(child_pipe);
    }
    else {
        close(child_pipe[0]);
        printf("prime %d\n", cur);
        int prime = cur;
        while(read(parent_pipe[0], &prime, sizeof(int)) != 0){
            if(prime % cur != 0){
                write(child_pipe[1], &prime, sizeof(int));
            }
        }
        close(child_pipe[1]);
        wait(0);
    }
    exit(0);

}

int main(int argc, char *argv[])
{
    int pd[2];
    int pid;

    pipe(pd);
    pid = fork();
    if(pid < 0)
    {
        exit(1);
    }
    if(pid == 0)
    {
        child(pd);
    }
    else{
        close(pd[0]);
        for(int i = 2; i <= SIZE; i++)
        {
            write(pd[1], &i, sizeof(int));
        }
        close(pd[1]);
        wait(0);
    }
    exit(0);

}