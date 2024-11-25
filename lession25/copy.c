/*
使用内存映射实现文件拷贝的功能
思路：
    1.对原始的文件进行内存映射
    2.创建一个新文件(拓展该文件)
    3.把新文件的数据映射到内存中
    4.通过内存拷贝将第一个文件的内存数据拷贝到新的文件内存中
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
    //memcpy和strcpy的区别在于memcpy不仅仅用于字符串的复制也可以用于二进制数据的拷贝
    int fd1=open("./test.txt",O_RDWR);
    int len=lseek(fd1,0,SEEK_END);
    int fdc=open("./copy.txt",O_CREAT|O_RDWR,0664);
    int res=ftruncate(fdc,len);
    write(fdc," ",1);
    void* ptr1=mmap(NULL,len,PROT_READ,MAP_SHARED,fd1,0);
    void* ptrc=mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fdc,0);
    if(ptr1==MAP_FAILED){
        perror("mmap");
    }
    if(ptrc==MAP_FAILED){
        perror("mmap");
    }
    memcpy(ptrc,ptr1,len);
    
    munmap(ptrc,len);
    munmap(ptr1,len);
    
    close(fdc);
    close(fd1);
    return 0;
}