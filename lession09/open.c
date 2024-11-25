/*
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
       //打开一个已经存在的文件
       int open(const char *pathname, int flags);
       参数：
            pahtname:要打开文件的路径
            flags:对文件的操作权限 O_RDONLY, O_WRONLY, or O_RDWR这三个操作是互斥的
       返回值：
            返回一个文件描述符 如果调用失败，返回-1 
       errno: 属于Linux系统函数库 库里面的全局变量 记录最近的错误号

       #include <stdio.h>

       void perror(const char *s);//作用：打印errno对应的错误秒速
            s参数：用户描述，比如hello，最终输出的内容是 hello:xxx(实际的错误描述)
       
       //创建一个新的文件
       int open(const char *pathname, int flags, mode_t mode);

*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
int main(){
    //打开一个文件
    int fd=open("a.txt",O_RDONLY);
    if(fd==-1) 
        perror("open failed:");
    //读写操作


    //关闭文件
    close(fd);
    return 0;
}