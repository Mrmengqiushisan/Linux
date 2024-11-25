/*
#include <unistd.h>
int chdir(const char *path);
    作用：修改进程的工作目录
#include <unistd.h>
char *getcwd(char *buf, size_t size);
    作用：获取当前工作目录
*/
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
int main(){
    char buf[128]={0};
    getcwd(buf,sizeof(buf));
    printf("当前的工作目录为：%s\n",buf);
    int ret=chdir("/home/wangtao/Linux/lession12");
    if(ret==-1)perror("chdir:");

    int fd=open("chdir.txt",O_CREAT|O_RDWR,0664);
    if(fd==-1)perror("fd:");
    close(fd);

    char buf1[128]={0};
    getcwd(buf1,sizeof(buf1));
    printf("当前的工作目录为：%s\n",buf1);
    
    return 0;
}