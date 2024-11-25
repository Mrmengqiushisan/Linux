//1秒钟电脑可以数多少个数
#include<unistd.h>
#include<stdio.h>
/*
    实际时间=内核时间+用户时间+消耗时间(IO)
    进行文件IO操作的时候比较浪费时间
    定时器，与进程的状态无关(自然定时法) 无论进程处于哪种状态 都会计时
*/
int main(){
    int i=0;
    alarm(1);
    while(1){
       printf("%i\n",i++);
    }
    return 0;
}