/*
#include <unistd.h>
int execlp(const char *file, const char *arg, ...);
    - 会到环境变量中查找指定的可执行文件
    - 参数：
        - file:需要执行的可执行文件的文件名
        - arg: 执行可执行文件所需要的参数列表
            第一个参数一般没有什么作用 为了方便 一般写的是执行的程序的名称
            从第二个参数开始就是程序执行所需要的列表
            参数最后需要以NULL结束(哨兵)
    - 返回值：
        只有调用错误时才会有返回值 返回-1 并且设置errno
*/
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
int main(){
    //创建一个子进程，在子进程中执行exec函数族中的函数
    pid_t pid=fork();
    if(pid>0){
        //父进程
        printf("i am parent process, pid: %d\n",getpid());
        sleep(1);
    }else if(pid==0){
        //子进程
        printf("i am child process, pid: %d\n",getpid());
        //execl("hello","hello",NULL);
        execlp("ps","ps","aux",NULL);
        printf("i am child process\n");
    }
    for(int i=0;i<3;i++){
        printf("i=%d, pid:%d\n",i,getpid());
    }

}