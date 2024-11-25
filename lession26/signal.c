/*
#include <signal.h>
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
    - 功能：设置某个信号的捕捉行为
    - 参数：
        - signum:要捕捉信号
        - handler：捕捉到信号要如何处理
            - SIG_IGN: 忽略信号
            - SIG_DFL: 使用信号默认的行为
            - 回调函数: 这个函数是内核调用 程序员只负责写 捕捉到信号后如何处理信号
            回调函数：
                - 需要程序员实现，提前准备号的，函数的类型根据实际需求，看函数指针的定义
                - 不是程序员调用，而是当信号产生，由内核调用
                - 函数指针是实现回调的手段，函数实现之后，将函数名放在函数指针的位置
    - 返回值:
        成功，返回上一次注册的信号处理函数的地址，第一次调用返回NULL
        失败, 返回SIG_ERR,设置错误号
    
The signals SIGKILL and SIGSTOP cannot be caught or ignored.
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
    //注册信号捕捉
    // signal(SIGALRM,SIG_IGN);
    // signal(SIGALRM,SIG_DFL);
    // typedef void (*sighandler_t)(int);函数指针，int类型的参数表示捕捉到的信号的值
    signal(SIGALRM,myALarm);

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