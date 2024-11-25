/*
#include <unistd.h>
unsigned int alarm(unsigned int seconds);
    - 功能：设置定时器(闹钟) 函数调用开始倒计时 当倒计时为0的时候
            函数会给当前进程发送信号SIGALARM
    - 参数：
        seconds:倒计时的时长，单位：秒 如果参数为0 定时器无效
                取消一个定时器 通过alarm(0)
    - 返回值：
        - 之前没有定时器，返回0
        - 之前有定时器，返回之前的定时器剩余的时间
    - SLGALARM: 默认终止当前的进程，每一个进程都有且只有一个定时器
        alarm(10); ->返回0
        过了1秒
        alarm(5);  ->返回9
    - 注意：该函数不阻塞的
*/
#include<unistd.h>
#include<stdio.h>

int main(){
    int second=alarm(5);
    printf("second = %d\n",second);
    sleep(2);
    second=alarm(2);
    printf("second = %d\n",second);
    while(1){

    }
    return 0;
}