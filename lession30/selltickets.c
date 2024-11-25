/*
使用多线程实现卖票案例
有3个窗口，一共是100张票

*/


#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

void* sellticket(void* arg){
    //买票
    static int ticketd=100;
    while(ticketd>0){
        usleep(3000);
        printf("%ld 正在卖 %d 张门票\n",pthread_self(),ticketd--);
    }
}

int main(){
    //创建三个子线程
    pthread_t tid[3]={0};
    for(int i=0;i<3;i++){
        tid[i]=pthread_create(tid,NULL,sellticket,NULL);
    }
    //回收子线程的资源 阻塞
    for(int i=0;i<3;i++){
        pthread_join(tid[i],NULL);
    }
    //设置线程分离
    // for(int i=0;i<3;i++){
    //     pthread_detach(tid[i]);
    // }
    //退出主线程
    pthread_exit(NULL);
}