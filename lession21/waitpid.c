/*
#include <sys/types.h>
#include <sys/wait.h>
pid_t waitpid(pid_t pid, int *wstatus, int options);
    功能：回收指定进程号的子进程,可以设置非阻塞
    参数：
        - pid:
            pid>0:某个子进程的pid
            pid=0:表示回收当前进程组的所有子进程    (也会用)
            pid=-1:回收所有的子进程，相当于调用wait() (最常用)
            pid<-1:回收某个进程组的组id的绝对值,回收指定进程组中的子进程
        - options:设置阻塞和非阻塞
            0：阻塞
            WNOHANG:非阻塞
    返回值: 
        >0:返回子进程的id
        =0:非阻塞的情况下：表示还有子进程活着
        =-1：错误，或者没有子进程活着
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
        while(1){
            sleep(1);
            printf("parent,pid=%d\n",getpid());
            int st;
            int ret=waitpid(-1,&st,WNOHANG);
            //int ret=wait(NULL);
            if(ret==-1)break;
            else if(ret==0)continue;
            else if(ret>0){
                if(WIFEXITED(st))
                    printf("退出的状态码：%d\n",WEXITSTATUS(st));
                if(WIFSIGNALED(st))
                printf("被哪个信号干掉了：%d\n",WTERMSIG(st));
                printf("child die pid=%d\n",ret);
            }
        }
    }else if(pid==0){
        //子进程
        while(1){
            printf("child,pid=%d\n",getpid());
            sleep(3);
        }
        exit(0);
    }
    return 0;//exit(0)
}