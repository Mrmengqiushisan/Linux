/*
void pthread_exit(void *retval);
    - 功能：终止一个线程 在哪个线程中调用 就表示终止哪个线程
    - 参数：
        retval:需要传递一个指针，作为一个返回值，可以在pthread_join()中找到
pthread_t pthread_self(void);
    - 功能：获取当前线程的线程ID
int pthread_equal(pthread_t t1, pthread_t t2);
    - 功能：比较两个线程id是否相等
    不同的操作系统，pthread_t 类型的实现不一样，有的是无符号的长整型，有的是使用
    结构体区实现的
*/

#include<pthread.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void* callback(void* arg){
    printf("child thread:%ld\n",pthread_self());
    return NULL;
}
int main(){
    pthread_t pid;
    int ret=pthread_create(&pid,NULL,callback,NULL);
    if(ret!=0){
        printf("pthread_create:%s\n",strerror(ret));
        exit(0);
    }
    for(int i=0;i<5;i++){
        printf("%d\n",i);
    }
    printf("tid: %ld ,main thread:%ld\n",pid,pthread_self());
    //让主线程退出，当主线程退出时，不会影响其他正常运行的线程
    pthread_exit(NULL);
    return 0;
}