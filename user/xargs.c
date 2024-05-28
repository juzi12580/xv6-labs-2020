#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

#define ARGSIZE 32

int main(int argc, char *argv[]){
    if(argc <= 1){
        fprintf(2, "usage should to many\n");
        exit(1);
    }
    char *arg_cmd = argv[1]; //记录输入的命令
    char argv_list[MAXARG][ARGSIZE]; //实际输入的命令行数组
    char *new_argv_list[MAXARG];
    char buf;               //输入在管道中的命令

     while(1){
        memset(argv_list, 0, MAXARG*ARGSIZE);

        for(int i = 1; i < argc; ++i){
            strcpy(argv_list[i-1], argv[i]); //将除了命令名的字符串，复制给argv_list
        }

        int offset = 0;
        int cur_argc = argc - 1;
        int read_stdin = 0;
    
        while((read_stdin = read(0, &buf, 1)) > 0) {
            
            if(buf == ' '){
                cur_argc++;
                offset = 0;
                continue;
            }
            if(buf == '\n'){
                break;
            }
            if(offset == ARGSIZE){
                fprintf(2, "parameter too long\n");
            }
            if(cur_argc == MAXARG){
                fprintf(2, "arg to many\n");
            }

            argv_list[cur_argc][offset++] = buf;
        }

        if(read_stdin <= 0){
            break;
        }
        
        //组装新的argv[]参数
        for(int i = 0; i <= cur_argc; ++i){
            new_argv_list[i] = argv_list[i];
        }

        
        if(fork() == 0)
        {
            exec(arg_cmd, new_argv_list);
            exit(1);
        }
        wait(0);
        
        
    }
    exit(0);
}

