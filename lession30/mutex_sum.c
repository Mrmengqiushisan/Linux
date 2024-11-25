/*
利用多线程
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>
#define THREAD_SIZE 100

long long num=0;
pthread_mutex_t mutex1;

void* num_incr(void* arg){
    for(int i=0;i<500000;i++){
        pthread_mutex_lock(&mutex1);
        num+=1;
        pthread_mutex_unlock(&mutex1);
    }
    pthread_exit(NULL);
}

void* num_dec(void* arg){
    for(int i=0;i<500000;i++){
        pthread_mutex_lock(&mutex1);
        num-=1;
        pthread_mutex_unlock(&mutex1);
    }
    pthread_exit(NULL);
}


int main(){
    //初始化互斥量
    pthread_mutex_init(&mutex1,NULL);
    //初始化线程ID
    pthread_t thread[THREAD_SIZE]={0};
    printf("num:%lld\n",num);
    //开启线程
    for(int i=0;i<THREAD_SIZE;i++){
        if(i%2){
            pthread_create(&thread[i],NULL,num_incr,NULL);
        }else{
            pthread_create(&thread[i],NULL,num_dec,NULL);
        }
    }
    //接管线程等待执行完成
    for(int i=0;i<THREAD_SIZE;i++){
        pthread_join(thread[i],NULL);
    }
    printf("num:%lld\n",num);
    //释放互斥量
    pthread_mutex_destroy(&mutex1);
    pthread_exit(NULL);
    return 0;
}