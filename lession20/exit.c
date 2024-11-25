/*
#include <stdlib.h>
void exit(int status);

#include <unistd.h>
void _exit(int status);

status参数：是进程退出时的一个状态信息，父进程回收子进程资源的时候可以获得

*/

#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>

int main(){
    printf("hello\n");
    printf("world\n");
    
    exit(0);

    return 0;
}