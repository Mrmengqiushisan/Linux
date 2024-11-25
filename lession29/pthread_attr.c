/*
int pthread_attr_init(pthread_attr_t *attr);
    - 初始化线程属性变量
int pthread_attr_destroy(pthread_attr_t *attr);
    - 释放线程属性的资源
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
    - 获取线程分离的状态
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
    - 设置线程分离的状态属性
*/

#include<pthread.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

void* callback(void* arg){
    printf("child thread id:%ld\n",pthread_self());
    for(int i=0;i<5;i++){
        printf("child :%d\n",i);
    }
    static int value=10;
    pthread_exit((void*)&value);//等价于 return &value
}

int main(){
    //创建一个线程属性变量
    pthread_attr_t attr;
    //初始化属性
    pthread_attr_init(&attr);
    //设置属性
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    
    pthread_t pid;
    int ret=pthread_create(&pid,&attr,callback,NULL);
#ifdef DEBUG
    if(ret!=0){
        printf("pthread_create:%s\n",strerror(ret));
        exit(0);
    }
#endif
    printf("tid: %ld ,main thread:%ld\n",pid,pthread_self());
    //获取线程栈的大小
    size_t size;
    pthread_attr_getstacksize(&attr,&size);
    printf("stackszie:%d\n",(int)size);
   for(int i=0;i<5;i++){
    printf("%d\n",i);
   }
   //释放线程属性资源
   pthread_attr_destroy(&attr);
    //让主线程退出，当主线程退出时，不会影响其他正常运行的线程
    pthread_exit(NULL);
    return 0;
}