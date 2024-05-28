#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int childpd[2], parentpd[2];
    int pid;
    char buf[1] = "a";
    if(pipe(childpd) < 0){
        fprintf(2, "pipe faild\n");
        exit(1);
    }
    if(pipe(parentpd) < 0){
        fprintf(2, "pipe faild\n");
        exit(1);
    }
    pid = fork();
    if(pid < 0)
    {
        close(childpd[0]);
        close(childpd[1]);
        close(parentpd[0]);
        close(parentpd[1]);
        fprintf(2, "fork faild\n");
        exit(1);
    }
    if(pid > 0) //父进程
    {
        close(childpd[1]);
        close(parentpd[0]);
        write(parentpd[1],"A", 1);
        if(read(childpd[0],buf,sizeof(buf)) > 0){
            fprintf(1, "%d: received pong\n",getpid());
            
        }
        wait((int *)0);
        exit(0);
    }
    else //子进程
    {
        close(childpd[0]);
        close(parentpd[1]);
        
        if(read(parentpd[0],buf,sizeof(buf)) > 0){
            fprintf(1, "%d: received ping\n",getpid());
            write(childpd[1],"B", 1);
        }
        exit(0);
    }

}