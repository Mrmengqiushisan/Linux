/*
#include <unistd.h>
#include <fcntl.h>
int fcntl(int fd, int cmd, ...);
参数:
    fd:表示需要操作的文件描述符
    cmd:表示对文件描述符进行如何操作
        1.F_DUPFD复制文件描述符 int ret=fcntl(fd,F_DUPFD)
        2.F_GETFL获取指定的文件描述符文件状态flag
            获取的flag和我们通过open函数传递的flag是一个东西
        3.F_SETFL设置文件描述符文件状态flag
            必选项：O_RDONLY O_WRONLY O_RDWR 不可以被修改
            可选项：O_APPEND 表示追加数据 
                   O_NOBLOCK 表示非阻塞 
        阻塞和非阻塞：描述的是函数调用的行为 add()

*/
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
int main(){
    //复制文件描述符
    // int fd=open("1.txt",O_RDONLY);
    // int ret=fcntl(fd,F_DUPFD);
    //修改或者获取文件状态
    int fd=open("1.txt",O_RDWR);
    if(fd==-1)perror("open:");
    //获取文件描述符的状态flag
    int flag=fcntl(fd,F_GETFL);
    if(flag==-1)perror("fcntl:");
    flag|=O_APPEND;
    //修改文件描述符的flag 给flag加入O_APPEND这个标记
    int ret=fcntl(fd,F_SETFL,flag);
    if(ret==-1)perror("fcntl:");
    char* str="nihao";
    write(fd,str,strlen(str));
    close(fd);
    return 0;
}