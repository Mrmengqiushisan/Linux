/*
    读写锁的类型 pthread_rwlock_t
    int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);
    int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
    int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);

    案例：8个线程操作同一个全局变量
    3个线程不定时的写这个全局变量 5个线程不定时的读这个全局变量
*/
#include<pthread.h>
#include<stdio.h>
#include<unistd.h>

int num=1;
pthread_mutex_t mutex;
pthread_rwlock_t rwlock;
void* writeNum(void* arg){
    while(1){
        pthread_rwlock_wrlock(&rwlock);
        num++;
        printf("++write,tid:%ld,num:%d\n",pthread_self(),num);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }
    pthread_exit(NULL);
}

void* readNum(void* arg){
    while(1){
        pthread_rwlock_rdlock(&rwlock);
        printf("==read,tid:%ld,num:%d\n",pthread_self(),num);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }
    pthread_exit(NULL);
}
int main(){
    pthread_rwlock_init(&rwlock,NULL);
    //创建三个写线程 5个读线程
    pthread_t wtids[3]={0};
    pthread_t rtids[5]={0};
    for(int i=0;i<3;i++)
        pthread_create(&wtids[i],NULL,writeNum,NULL);
    for(int i=0;i<5;i++)
        pthread_create(&rtids[i],NULL,readNum,NULL);
    

    //设置线程分离
    for(int i=0;i<3;i++)
        pthread_detach(wtids[i]);
    for(int i=0;i<5;i++)
        pthread_detach(rtids[i]);

    pthread_rwlock_destroy(&rwlock);
    pthread_exit(NULL);
    return 1;
}