/*
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
int stat(const char *pathname, struct stat *statbuf);
    作用：获取一个文件相关的一些信息
    参数：
        - pathname:操作的文件的路径
        - statbuf:结构体变量，传出参数 用于保存获取到的文件信息
    返回值:成功返回0 失败返回-1 并且设置errno
int lstat(const char *pathname, struct stat *statbuf);
    作用：获取一个软链接文件相关的一些信息
    参数：
        - pathname:操作的文件的路径
        - statbuf:结构体变量，传出参数 用于保存获取到的文件信息
    返回值:成功返回0 失败返回-1 并且设置errno
*/
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>

int main(){
    struct stat statbuf;
    int ret=stat("a.txt",&statbuf);
    if(ret==-1){
        perror("stat failed:");
        return -1;
    }
    printf("stat size: %1ld\n",statbuf.st_size);
    return 1;
}