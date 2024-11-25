#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<string.h>

int main(){
    int pipefd[2];
    int ret=pipe(pipefd);
    if(ret==-1){
        perror("pipe:");
        exit(0);
    }
    long size=fpathconf(pipefd[0],_PC_PIPE_BUF);
    printf("pipe size :%ld\n",size);
    return 0;
}