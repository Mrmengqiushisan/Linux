/*
#include <unistd.h>
int access(const char *pathname, int mode);
    作用：判断某个文件是否有某个权限，或者判断是否存在
    参数：
        - pathname 判断的文件路径
        - mode 
            R_OK：读权限
            W_OK：写权限
            X_OK：执行权限 
            F_OK:文件是否存在
    返回值：成功返回0 失败返回-1

*/

#include<unistd.h>
#include<stdio.h>

int main(){
    int ret=access("a.txt",F_OK);
    if(ret==-1){
        perror("access failed:");
    }
    printf("文件存在！！\n");
    return 0;
}