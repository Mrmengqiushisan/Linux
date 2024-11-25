/*
    int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
        - 功能: 将自定义信号集中的数据设置到内核中(设置阻塞，解除阻塞，替换)
        - 参数:
            - how: 如何对内核阻塞信号集进行处理
                SIG_BLOCK:将用户设置的阻塞信号集添加到内核中，内核原来的数据不变
                    假设内核中默认的阻塞信号集是mask: mask|set
                SIG_UNBLOCK:根据用户设置的数据,对内核中的数据进行解除阻塞
                    mask &=~set
                SIG_SETMASK:覆盖内核中原来的值
            - set:已经初始化号的用户自定义的信号集
            - oldset:保存设置之前的内核中的阻塞信号集的状态 可以是NULL
        - 返回值：
            成功：0
            失败：-1
        
    int sigpending(sigset_t *set);
        - 功能：获取内核中的未决信号集
        - 参数：set 传出参数 返回内核中的未决信号集
*/

#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
//编写一个程序 把所有的常规信号的(1-31)的未决状态打印到屏幕
//设置某些信号是阻塞的，通过键盘产生这些信号
int main(){
    //设置 2 3 信号阻塞
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGINT);
    sigaddset(&set,SIGQUIT);
    //修改内核中的阻塞信号集
    sigprocmask(SIG_BLOCK,&set,NULL);
    int num=0;
    while(1){
        num++;
        //获取当前未决信号集的数据
        sigset_t pendingset;
        sigemptyset(&pendingset);
        sigpending(&pendingset);
        for(int i=1;i<=31;i++){
            if(sigismember(&pendingset,i)==1){
                printf("1");
            }else if(sigismember(&pendingset,i)==0){
                printf("0");
            }else{
                perror("sigismember");
                exit(0);
            }
        }
        sleep(1);
        printf("\n");
        if(num==10){
            sigprocmask(SIG_UNBLOCK,&set,NULL);
        }
    }
    return 1;
}