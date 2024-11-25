/*
互斥量的类型 pthread_mutex_t
int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
    - 初始化互斥量
    - 参数：
        - mutex:需要初始化的互斥量变量
        - attr:互斥量相关的属性,NULL
    - restrict:C语言的修饰符，被修饰的指针不能由另外的指针进行操作
        pthread_mutex_t *restrict mute = xxx;
        pthread_mutex_t * mutex1 = mutex; 报错
int pthread_mutex_destroy(pthread_mutex_t *mutex);
    - 释放互斥量的资源
int pthread_mutex_lock(pthread_mutex_t *mutex);
    - 加锁 阻塞的 如果有一个线程加锁了 那么其他的线程只能阻塞等待

int pthread_mutex_trylock(pthread_mutex_t *mutex);
    - 尝试加锁 如果加锁失败 不会阻塞 会直接返回
int pthread_mutex_unlock(pthread_mutex_t *mutex);
    - 解锁
*/

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
        tid[i]=pthread_create(&tid[i],NULL,sellticket,NULL);
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