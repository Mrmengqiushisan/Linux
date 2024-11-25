#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(){
    //打开管道文件
    int fd=open("./test",O_RDONLY);
    if(fd==-1){
        perror("open");
        exit(0);
    }
    while(1){
        char buf[1024]={0};
        int len=read(fd,buf,sizeof(buf));
        if(len==0){
            printf("写端断开连接...\n");
            break;
        }
        printf("recv buf:%s\n",buf);
        memset(buf,0,sizeof(buf));
    }
    return 0;
}