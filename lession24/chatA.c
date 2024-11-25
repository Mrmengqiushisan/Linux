#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
int main(){
    //判断有名管道文件是否存在
    int ret=access("fifo1",F_OK);
    if(ret==-1){
        printf("管道不存在 创建对应的有名管道\n");
        ret=mkfifo("fifo1",0664);
        if(ret==-1){
            perror("mkfifo");
            exit(0);
        }
    }
    ret=access("fifo2",F_OK);
    if(ret==-1){
        printf("管道不存在 创建对应的有名管道\n");
        ret=mkfifo("fifo2",0664);
        if(ret==-1){
            perror("mkfifo");
            exit(0);
        }
    }
    //以只写的方式打开管道一
    int fdw=open("fifo2",O_WRONLY);
    if(fdw==-1){
        perror("open");
        exit(0);
    }
    printf("打开FIFO2成功，等待写入...\n");
    //以只读的方式打开管道二
    int fdr=open("fifo1",O_RDONLY);
    if(fdr==-1){
        perror("open");
        exit(0);
    }
    printf("打开FIFO1成功，等待读取...\n");
    //循环的写端数据
    char buf[128]={0};
    while(1){
        memset(buf,0,sizeof(buf));
        //获取标准输入的数据
        char* s=fgets(buf,sizeof(buf),stdin);
        if(s==NULL){
            perror("fgets:");
            exit(0);
        }
        //写数据
        ret=write(fdw,buf,strlen(buf));
        if(ret==-1){
            perror("write");
            exit(0);
        }
        //读数据
        memset(buf,0,sizeof(buf));
        ret=read(fdr,buf,sizeof(buf));
        if(ret<=0){
            perror("read");
            exit(0);
        }
        printf("ChatB: %s",buf);
    }
    //关闭文件描述符
    close(fdr);
    close(fdw);
    return 0;
}