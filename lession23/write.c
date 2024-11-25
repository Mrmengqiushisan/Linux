#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
/*
有名管道的注意事项：
    1、一个为只读而打开一个管道的进程会阻塞，直到另外一个进程为只写打开管道
    2、一个为只写而打开一个管道的进程会阻塞，直到另外一个进程为只读打开管道
读管道：
    管道中有数据，read返回实际读到的字节数
    管道中无数据：
        管道写端被全部关闭，read返回0
        写端没有全部关闭，read阻塞等待
写管道：
    管道读端被全部关闭，进程异常终止(收到一个SIGPIPE信号)
    管道读端没有全部关闭：
        管道已经满了，write会阻塞
        管道没有满，write将数据写入，并返回写入实际的字节数
*/
int main(){
    //判断文件是否存在
    int ret=access("./test",F_OK);
    if(ret==-1){
        printf("管道不存在 创建管道\n");
        int ret=mkfifo("./test",0664);
        if(ret==-1){
            perror("mkfifo");
            exit(0);
        }
    }
    //以只写的方式打开管道
    int fd=open("./test",O_WRONLY);
    if(fd==-1){
        perror("open");
        exit(0);
    }
    //写数据
    for(int i=0;i<100;i++){
        char buf[1024]={0};
        sprintf(buf,"hello, %d\n",i);
        printf("write data: %s\n",buf);
        write(fd,buf,strlen(buf));
        sleep(1);
    }
    close(fd);
    return 0;
}