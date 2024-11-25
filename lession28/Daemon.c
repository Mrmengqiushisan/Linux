/*
    写一个守护进程  每隔2s获取一下系统时间，将这个时间写入磁盘文件中
*/
#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/time.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<signal.h>
#include<time.h>
#include<string.h>
void work(int num){
    //捕捉到信号 获取系统时间  写入磁盘文件
    time_t tm=time(NULL);
    struct tm* local= localtime(&tm);
    // char buf[1024]={0};
    // sprintf(buf,"%d-%d-%d %d:%d:%d\n",local->tm_year,local->tm_mon,local->tm_mday,
    // local->tm_hour,local->tm_min,local->tm_sec);
    char* str=asctime(local);
    int fd=open("time.txt",O_RDWR|O_CREAT|O_APPEND,0664);
    write(fd,str,strlen(str));
}

int main(){
    //1.创建子进程 退出父进程
    pid_t pid=fork();
    if(pid>0){
        exit(0);
    }
    //2.将子进程重新创建一个会话 
    setsid();
    //3.设置掩码
    umask(022);
    //4.更改工作目录
    chdir("/home/wangtao/");
    //5.关闭，重定向文件描述符
    int fd=open("/dev/null",O_RDWR);
    dup2(fd,STDIN_FILENO);
    dup2(fd,STDOUT_FILENO);
    dup2(fd,STDERR_FILENO);
    //6 业务逻辑
    //创建定时器
    //捕捉定时信号
    struct sigaction act;
    act.sa_flags=0;
    act.sa_handler=work;
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM,&act,NULL);
    struct itimerval val;
    val.it_value.tv_sec=2;
    val.it_value.tv_usec=0;
    val.it_interval.tv_sec=2;
    val.it_interval.tv_usec=0;
    setitimer(ITIMER_REAL,&val,NULL);
    while(1){
        sleep(10);
    }
    return 1;
}