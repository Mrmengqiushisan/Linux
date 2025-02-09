/*
#include <sys/time.h>
int setitimer(int which, const struct itimerval *new_value,struct itimerval *old_value);
    - 功能:设置定时器(闹钟)，可以替代alarm函数 精度比alarm高 微妙 实现周期性的定时
    - 参数：
        - which：定时器以什么时间计时
            ITIMER_REAL:真实时间，时间到达，发送SIGALARM 常用
            ITIMER_VIRTUAL:用户时间，时间到达，发送SIGVTALRM
            ITIMER_PROF:以该进程以用户态和内核态下所消耗的时间来计算，时间到达，发送SIGPROF
        - *new_value:设置定时器的属性
        struct itimerval {//定时器的结构体
            struct timeval it_interval; //每个阶段的时间，间隔时间
            struct timeval it_value;    //延迟多长时间执行定时器
        };
        struct timeval {//时间的结构体
            time_t      tv_sec;         //秒数
            suseconds_t tv_usec;        //微妙
        };
        过10秒后，每隔2秒定时一次
        - *old_value:记录上一次定时的时间参数 一般不使用 指定NULL
    - 返回值：
        成功：0
        失败：-1
*/ 
#include<sys/time.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
//过3秒后，每隔2秒调用一次
int main(){
    struct itimerval new_value;
    //设置间隔时间 2秒
    new_value.it_interval.tv_sec=2;
    new_value.it_interval.tv_usec=0;
    //设置延迟的时间 3秒之后开始发送信号
    new_value.it_value.tv_sec=3;
    new_value.it_value.tv_usec=0;
    int ret=setitimer(ITIMER_REAL,&new_value,NULL);
    printf("定时器开始了:\n");
    if(ret==-1){
        perror("setitimer:");
        exit(0);
    }
    getchar();
    return 0;
}