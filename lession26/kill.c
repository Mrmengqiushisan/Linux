/*
#include <sys/types.h>
#include <signal.h>
int kill(pid_t pid, int sig);
    - 功能：给任何的进程或者进程组发送pid 发送某个信号 sig
    - 参数： 
        -pid:需要发送给的进程的id
            >0:将信号发送给指定的进程
            =0；将信号发送给当前的进程组
            =-1：将信号发送给每一个有权限接受这个信号的进程
            <-1: 这个pid=某个进程组的ID取反
        -sig:需要发送信号的编号(尽量使用宏值)0表示不发送任何信号
    kill(getppid(),9)
    kill(getpid(),9)
int raise(int sig);
    - 功能：给当前进程发送信号
    - 参数：
        -sig:要发送的信号
    - 返回值：
        - 成功 0
        - 失败 非0
void abort(void);
    - 功能：发送SIGABRT信号给当前的进程,杀死当前进程
    kill(getpid(),SIGABRT)
*/

#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    pid_t pid=fork();
    if(pid>0){
        //父进程
        printf("parent peocess\n");
        sleep(2);
        printf("kill child process now\n");
        kill(pid,SIGINT);
    }else if(pid==0){
        //子进程
        int i=0;
        for(int i=0;i<5;i++){
            printf("child process\n");
            sleep(1);
        }
    }

    return 0;
}
