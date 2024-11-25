#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main(){
    //创建子进程
    pid_t pid=fork();
    if(pid>0){
        while(1){
            //如果大于0 返回的是创建的子进程的进程号
            printf("i am parent process,pid: %d,ppid:%d\n",getpid(),getppid());
            sleep(1);
        }
    }else if(pid==0){
        sleep(1);
        //当前是子进程
        printf("i am child process pid: %d,ppid:%d\n",getpid(),getppid());
    }
    for(int i=0;i<5;i++){
        printf("%d ,pid: %d\n",i,getpid());
    }
    return 0;
}