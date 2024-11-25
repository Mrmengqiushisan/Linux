/*
SIGCHLD信号产生的条件
    子进程终止时
    子进程接收到 SIGSTOP 信号停止时
    子进程处在停止态，接受到SIGCONT后唤醒时
*/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>

void myFun(int a){
    printf("捕捉到的信号：%d\n",a);
    while (1){
        int ret=waitpid(-1,NULL,WNOHANG);
        if(ret>0){
            printf("chaild die,pid=%d\n",ret);
        }else if(ret==0){
            break;
        }else if(ret==-1){
            //没有
            break;
        }
    }
}

int main(){
    //提前设置号阻塞信号集SIGCHLD 因为有可能子进程很快结束 父进程还没有注册完成
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGCHLD);
    sigprocmask(SIG_BLOCK,&set,NULL);
    pid_t pid;
    for(int i=0;i<20;i++){
        pid=fork();
        if(pid==0)break;
    }
    if(pid>0){
        struct sigaction act;
        act.sa_flags=0;
        act.sa_handler=myFun;
        sigemptyset(&act.sa_mask);
        sigaction(SIGCHLD,&act,NULL);
        //注册完信号捕捉以后
        sigprocmask(SIG_UNBLOCK,&set,NULL);
        while(1){
            //父进程
            //捕捉子进程死亡时发送的SIGCHLD信号
            printf("parent process pid:%d\n",getpid());
            sleep(2);
        }
    }else if(pid==0){
        printf("child process pid:%d\n",getpid());
    }
    return 0;
}