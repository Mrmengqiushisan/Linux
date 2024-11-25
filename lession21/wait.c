/*
#include <sys/types.h>
#include <sys/wait.h>
pid_t wait(int *wstatus);
    功能：等待任意一个子进程结束，如何任意一个子进程结束了，次函数会回收子进程的资源
    参数: int *wstatus
    进程退出的状态信息，传入的是一个int类型的地址，传出参数
    返回值：
        - 成功 返回的子进程的id
        - 失败 返回-1(所有的子进程都结束 调用函数失败)
调用wait函数的进程会被挂起(阻塞),直到它的一个子进程退出或者收到一个不能被忽略的信号时才被唤醒(相当于继续往下执行)
如果没有子进程，函数立刻返回，返回-1 如果子进程都已经结束了，也会立即返回 返回-1
pid_t waitpid(pid_t pid, int *wstatus, int options);

*/

#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(){
    //有一个父进程，创建5个子进程
    pid_t pid;
    for(int i=0;i<5;i++){
        pid=fork();
        if(pid==0){
            break;
        }
    }
    if(pid>0){
        //父进程
        printf("parent,pid=%d\n",getpid());
        while(1){
            int st;
            int ret=wait(&st);
            //int ret=wait(NULL);
            if(ret==-1){
                printf("wait failed\n");
                break;
            }
            if(WIFEXITED(st)){
                printf("退出的状态码：%d\n",WEXITSTATUS(st));
            }
            if(WIFSIGNALED(st)){
                printf("被哪个信号干掉了：%d\n",WTERMSIG(st));
            }
            printf("child die pid=%d\n",ret);
            sleep(1);
        }
    }else if(pid==0){
        //子进程
        printf("child,pid=%d\n",getpid());
        sleep(1);
        // while(1){
        //     printf("child,pid=%d\n",getpid());
        //     sleep(1);
        // }
        exit(1);
    }
    printf("who am i? pid=%d\n",getpid());
    return 0;//exit(0)
}