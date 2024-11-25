#include<sys/mman.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<wait.h>
int main(){
    //打开一个文件
    int fd=open("./test.txt",O_RDWR);
    int size=lseek(fd,0,SEEK_END);
    //创建内存映射区
    void* ptr=mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(ptr==MAP_FAILED){
        perror("mmap");
        exit(0);
    }
    int i=0;
    char buf[64]={0};
    while(1){
        sprintf(buf,"Hello Chat A:%d\n",i);
        strcpy((char*)ptr,buf);
        i++;
    }
    //关闭内存映射区
    munmap(ptr,size);
    return 0;
}