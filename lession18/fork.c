/*
#include <sys/types.h>
#include <unistd.h>
pid_t fork(void);
    函数作用：用于创建子进程
    返回值：
        fork()的返回值会返回两次：一次是在父进程中，一次是在子进程中
        在父进程中返回创建的子进程的ID 在子进程中返回0
        如何区分父进程和子进程：通过fork的返回值
        在父进程中返回-1，表示创建子进程失败，并且设置errno
    父子进程之间的关系：
    区别：
        1.fork()函数的返回值不同
            父进程中：>0 返回的子进程ID
            子进程中：=0
        2 PCB中的一些数据
            当前进程的id pid
            当前的进程的父进程的id ppid
    共同点：
        某些状态下：子进程刚被创建出来，还没有执行任何的写数据的操作
            - 用户区的数据
            - 文件描述符
    父子进程对变量是不是共享的？
        - 刚开始的时候 是一样的 共享的 如果修改了数据 就不共享了
        - 读时共享(写了之后就不共享了)
*/
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main(){
    //创建子进程
    pid_t pid=fork();
    if(pid>0){
        printf("pid:  %d\n",pid);
        //如果大于0 返回的是创建的子进程的进程号
        printf("i am parent process,pid: %d,ppid:%d\n",getpid(),getppid());
    }else if(pid==0){
        //当前是子进程
        printf("i am child process pid: %d,ppid:%d\n",getpid(),getppid());
    }

    for(int i=0;i<5;i++){
        printf("%d ,pid: %d\n",i,getpid());
        sleep(1);
    }
    return 0;
}