/*
创建fifo文件
通过命令：mkfifo 
#include <sys/types.h>
#include <sys/stat.h>
通过系统函数：int mkfifo(const char *pathname, mode_t mode);
    参数：
        - pathname:管道名称的路径
        - mode:文件的权限 和open的mode是一样的
    返回值：
        - 成功：0
        - 失败：-1
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int main(){
    //判断文件是否存在
    int ret=access("fifo1",F_OK);
    if(ret==-1){
        printf("管道不存在 创建管道\n");
        int ret=mkfifo("./fifo1",0664);
        if(ret==-1){
            perror("mkfifo");
            exit(0);
        }
    }
    return 0;
}