/*
Linux 内核函数
#include <sys/types.h>
#include <unistd.h>
off_t lseek(int fd, off_t offset, int whence);
    - fd open得到 文件描述符 通过fd操作某个文件
    - offset 偏移量
    - whence 标记
        SEEK_SET 设置文件指针的偏移量
        SEEK_CUR  设置偏移量：当前位置+第二个参数offset的值
        SEEK_END  设置偏移量：文件大小+第二个参数offset的值
返回值：返回文件指针的位置
作用：
    移动文件指针到头文件 lseek(fd,0,SEEK_SET);
    获取当前文件指针的位置：lseek(fd,0,SEEK_CUR);
    可以获取文件的长度：lseek(fd,0,SEEK_END);
    拓展文件的长度，当前文件为10b 110b 增加了100个字节：lseek(fd,100,SEEK_END);
    注意:需要写一次才管用
标准C函数
#include <stdio.h>
int fseek(FILE *stream, long offset, int whence);
*/
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>

int main(){
    int fd=open("hello.txt",O_RDWR);
    if(fd==-1)perror("open failed:");
    off_t ret=lseek(fd,100,SEEK_SET);
    if(ret==-1)perror("write failed:");
    write(fd," ",1);
    close(fd);
    return 0;
}