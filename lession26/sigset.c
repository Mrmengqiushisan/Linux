/*
以下信号集相关的函数都是对自定义的信号集进行操作
int sigemptyset(sigset_t *set);
    - 功能: 清空信号集中的数据 将信号集中的所有的标志位置为0
    - 参数：set 传出参数 需要造作的信号集
    - 返回值：0 成功 -1 失败
int sigfillset(sigset_t *set);
    - 功能: 将信号集中的所有的标志为置为1
    - 参数：set 传出参数 需要造作的信号集
    - 返回值：0 成功 -1 失败
int sigaddset(sigset_t *set, int signum);
    - 功能: 设置信号集中的某一个对应得标志位为1 表示阻塞这个信号
    - 参数：
        - set 传出参数 需要造作的信号集
        - signum:需要设置阻塞的那个信号
    - 返回值：0 成功 -1 失败
int sigdelset(sigset_t *set, int signum);
    - 功能: 设置信号集中的某一个对应得标志位为0 表示不阻塞这个信号
    - 参数：
        - set 传出参数 需要造作的信号集
        - signum:需要设置不阻塞的那个信号
    - 返回值：0 成功 -1 失败
int sigismember(const sigset_t *set, int signum);
    - 功能: 判断某个信号是否阻塞
    - 参数：
        - set 需要操作的信号集
        - signum:需要设置阻塞的那个信号
    - 返回值：
        - 1：signum被阻塞
        - 0：signum不阻塞
        - -1：signum调用失败
*/

#include<stdio.h>
#include<signal.h>

int main(){
    //创建一个信号集
    sigset_t set;
    //清空信号集
    sigemptyset(&set);
    //判断 SIGINT 是否在信号集中
    int ret=sigismember(&set,SIGINT);
    if(ret==0){
        printf("SIGIN 不阻塞\n");
    }else if(ret==1){
        printf("SIGIN 阻塞\n");
    }else{
        perror("sigismember");
    }
    //添加信号到信号集中
    sigaddset(&set,SIGINT);
    sigaddset(&set,SIGQUIT);
    ret=sigismember(&set,SIGINT);
    if(ret==0){
        printf("SIGIN 不阻塞\n");
    }else if(ret==1){
        printf("SIGIN 阻塞\n");
    }else{
        perror("sigismember");
    }
    //从信号集删除信号
    sigdelset(&set,SIGQUIT);
    ret=sigismember(&set,SIGQUIT);
    if(ret==0){
        printf("SIGQUIT 不阻塞\n");
    }else if(ret==1){
        printf("SIGQUIT 阻塞\n");
    }else{
        perror("sigismember");
    }
    return 0;
}