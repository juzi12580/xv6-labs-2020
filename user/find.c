#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


void find(char *path, char* file_name){
    char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st; //包含文件的信息

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

//fstat把所有文件状态复制到 st结构体中
  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    if(de.inum == 0){
        continue;
    }
    memmove(p, de.name, DIRSIZ); //将 de.name复制 DIRSIZ个字符到p,它比memcpy()更安全，如果目标区域和源区域有重叠复制后源区域的内容会改变
    p[DIRSIZ] = 0;
    if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
    }
    switch(st.type){
        case T_FILE:{
            if(strcmp(de.name, file_name) == 0){
                printf("%s\n", buf);
            }
            break;
        }
        case T_DIR:{
            if(strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0){
                find(buf, file_name);
            }
            break;
        }
    }
  }

  close(fd);
}

int main(int argc, char *argv[])
{

    if(argc != 3){
        printf("there should be one path argument and one filename argument\n");
        exit(0);
    }
    char* path_name = argv[1];
    char* file_name = argv[2];
    find(path_name, file_name);
    exit(0);
}