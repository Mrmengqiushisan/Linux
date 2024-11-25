#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

int main(int argc,char* argv[]){
    pid_t pid=fork();
    if(pid>0){
        //父进程
        printf("parent,pid:%d; my child pid: %d\n",getpid(),pid);
        sleep(1);
        printf("parent exited\n");
    }else if(pid==0){
        //子进程
        printf("child pid:%d;my parent pid:%d\n",getpid(),getppid());
        sleep(30);
        printf("after 30 second\n");
        printf("child pid: %d,my parent pid:%d\n",getpid(),getppid());
        exit(0);
    }
    return 0;
}