/*
    #include <unistd.h>

    ssize_t read(int fd, void *buf, size_t count);
        参数：
            - fd 文件描述符 open得到的 通过文件描述符操作某个文件
            - buf 需要读取数据存放的地方,数组的地址(传出参数)
            - count 指定的数组的大小
        返回值:
            - 成功：
                >0:返回实际的读取到的字节数
                =0：文件已经读取完了
            - 失败：-1 并且设置errno
    
    #include <unistd.h>

    ssize_t write(int fd, const void *buf, size_t count);
       参数：
            - fd:文件描述符 open得到的 通过文件描述符操作某个文件
            - buf:要往磁盘写入的数据，数据
            - count:要写数据的实际的大小
       返回值：
            - 成功：实际写入的字节数
            - 失败：返回-1 并且设置errno
*/
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int main(){
    //打开文件
    int frd=open("config_record.txt",O_RDONLY);
    if(frd==-1)perror("open failed:");
    //创建一个新的文件
    int fwd=open("config_record_copy.txt",O_RDWR|O_CREAT,0775);
    if(fwd==-1)perror("create failed:");
    //读写操作
    char buffer[1024]={};
    int len=0;
    while((len=read(frd,buffer,sizeof(buffer)))>0){
        write(fwd,buffer,len);
    }
    //关闭文件
    close(frd);
    close(fwd);
    return 0;
}