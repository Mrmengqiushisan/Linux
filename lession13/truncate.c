/*
#include <unistd.h>
#include <sys/types.h>
int truncate(const char *path, off_t length);
    作用:缩减或者扩展文件的尺寸至指定的大小
*/
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
int main(){
    int ret=truncate("a.txt",20);
    if(ret==-1)
        perror("truncate:");
    return 0;
}