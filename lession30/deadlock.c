

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>
pthread_mutex_t mutex;
int ticketd=1000;
void* sellticket(void* arg){
    //买票
    while(1){
        pthread_mutex_lock(&mutex);
        if(ticketd>0){
            printf("%ld 正在卖 %d 张门票\n",pthread_self(),ticketd--);
        }
        else {
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main(){
    //创建三个子线程
    int ret=pthread_mutex_init(&mutex,NULL);
#ifdef DEBUG
    if(ret==-1){
        printf("pthread_mutex_init:%s\n",strerror(ret));
        exit(0);
    }
#endif
    pthread_t tid[10]={0};
    for(int i=0;i<10;i++){
        pthread_create(&tid[i],NULL,sellticket,NULL);
    }
    //回收子线程的资源 阻塞
    for(int i=0;i<10;i++){
        pthread_join(tid[i],NULL);
    }
    //释放互斥量资源
    pthread_mutex_destroy(&mutex);
    //退出主线程
    pthread_exit(NULL);
}