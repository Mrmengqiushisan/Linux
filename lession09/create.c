/*
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>

    //创建一个新的文件
    int open(const char *pathname, int flags, mode_t mode);
    参数：
         - pathname 打开的文件路径
         - flags 对文件的操作权限
            -必选项：O_RDONLY, O_WRONLY, or O_RDWR这三个操作是互斥的
            -可选项：O_APPEND O_CREAT 
         flags参数是一个int类型的数据 占4个字节 32位
         flags32位 每一位就是一个标志位
         - mode:八进制的数，表示创建出的新的文件的操作权限 比如：0775
         最终的权限是：mode&~umask 
         0777   ->  111111111
         0775   ->  111111101
         ---------------------
         0775       111111101
         umask的作用就是抹去某些权限
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
int main(){

    int fd=open("create.txt",O_RDWR|O_CREAT,0777);
    if(fd==-1)perror("open failed:");
    close(fd);
    return 0;
}