/*
#include <signal.h>
int sigaction(int signum, const struct sigaction *act,struct sigaction *oldact);
    - 功能:检查或者改变信号的处理 信号捕捉
    - 参数:
        - signum:需要捕捉的信号的编号或者宏值
        - *act: 捕捉到信号之后的处理动作
        - oldact: 上一次对信号捕捉相关的设置 一般不适用 传递 NULL
    - 返回值:
        成功 0
        失败 -1
    struct sigaction {
        //函数指针 指向的函数是信号捕捉到之后的处理函数
        void     (*sa_handler)(int);
        //函数指针 不常用
        void     (*sa_sigaction)(int, siginfo_t *, void *);
        //临时阻塞信号集 信号捕捉函数执行过程中 临时阻塞某些信号
        sigset_t   sa_mask;
        //使用哪一个信号处理对捕捉的信号进行处理
        //这个值可以是0 表示使用 sa_handler 也可以是 SA_SIGINFO表示使用sa_sigaction 
        int        sa_flags;
        //被废弃掉了
        void     (*sa_restorer)(void);
    };
    - 注意:应该尽量使用sigaction()因为signal()是ANSI美国国家标准定义的，有些架构已经废弃
*/

#include<sys/time.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
//过3秒后，每隔2秒调用一次
long long i=0;
void myALarm(int num){
    printf("捕捉到了信号的编号是：%d\n",num);
    printf("每秒计算：%lld\n",i);
}
int main(){
    struct sigaction act;
    act.sa_flags=0;
    act.sa_handler=myALarm;
    sigemptyset(&act.sa_mask);//清空临时阻塞信号集
    //注册信号捕捉
    sigaction(SIGALRM,&act,NULL);
    struct itimerval new_value;
    //设置间隔时间 2秒
    new_value.it_interval.tv_sec=1;
    new_value.it_interval.tv_usec=0;
    //设置延迟的时间 3秒之后开始发送信号
    new_value.it_value.tv_sec=1;
    new_value.it_value.tv_usec=0;
    int ret=setitimer(ITIMER_REAL,&new_value,NULL);
    printf("定时器开始了:\n");
    if(ret==-1){
        perror("setitimer:");
        exit(0);
    }
    sleep(1);
    while(1){
        i++;
    }
    return 0;
}