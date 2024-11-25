/*
#include <unistd.h>
struct fd_pair {long fd[2];};
struct fd_pair pipe();
int pipe(int pipechildfd[2]);
    作用：创建一个匿名管道
    参数：int pipechildfd[2]这个数组是一个传出参数
        pipechildfd[0]对应的是管道的读端
        pipechildfd[1]对应的是管道的写端
    返回值：
        成功：0
        失败：-1
    管道是默认阻塞的：如果管道中没有数据 read阻塞，如果管道满了 write阻塞
    注意：匿名管道只能用于具有关系的进程之间的通信(父子进程，兄弟进程)
*/

#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<string.h>
//子进程发送数据给父进程，父进程读取数据
int main(){
    //在fork之前创建管道
    int pipechildfd[2];
    int id=pipe(pipechildfd);
    if(id==-1){
        perror("pipe:");
        exit(0);
    }
    pid_t pid=fork();
    if(pid>0){
        //父进程
        //关闭写端
        close(pipechildfd[1]);
        //从管道的读取段读数据
        char buf[1024]={0};
        while(1){
            read(pipechildfd[0],buf,sizeof(buf));
            printf("parent recv:%s,pid:%d\n",buf,getpid());
        }
    }else if(pid==0){
        //子进程
        close(pipechildfd[0]);
        char buf[1024]={0};
        while(1){
            char* str="hello,i am child";
            write(pipechildfd[1],str,strlen(str));
            sleep(1);
        }
    }
    return 0;
}