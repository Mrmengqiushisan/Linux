/*
#include <sys/types.h>
#include <unistd.h>
pid_t getpgrp(void);
    功能: 获取当亲进程组标识
pid_t getpgid(pid_t pid);
    功能: 获取进程号pid所指定的组id
int setpgid(pid_t pid, pid_t pgid);
    功能: 将进程号pid的组id设置为pgid
pid_t getsid(pid_t pid);
    功能：获取进程号为pid的会话id
pid_t setsid(void);
    功能：设置会话id、
*/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
int main(){
    __pid_t pid=getpid();
    printf("当前进程id为：%d\n",pid);
    int gpid=getpgid(pid);
    printf("当前进程组id为：%d\n",gpid);
    int sid=getsid(pid);
    printf("当前进程会话id为: %d\n",sid);
    int ret=setpgid(pid,2024);
    if(ret==-1){
        perror("setpgid:");
        exit(0);
    }
    printf("当前进程组id为：%d\n",getpgid(pid));
    return 0;
}