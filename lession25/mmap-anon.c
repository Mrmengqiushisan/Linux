/*
    匿名映射: 不需要文件实体进程一个内存映射

*/

#include<sys/mman.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<wait.h>

int main(){
    //1 创建匿名内存映射区
    int length=4096;
    void* ptr=mmap(NULL,length,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    if(ptr==MAP_FAILED) perror("mmap");
    //父子进程之间的通信
    pid_t pid=fork();
    if(pid>0){
        strcpy((char*)ptr,"hello world");
        wait(NULL);
    }else if(pid==0){
        //子进程 读取数据
        sleep(1);
        printf("%s\n",(char*)ptr);
    }
    int ret=munmap(ptr,length);
    if(ret==-1){
        perror("munmap");
    }
    return 0;
}