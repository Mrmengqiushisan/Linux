/*
#include <pthread.h>
int pthread_cancel(pthread_t thread);
    - 功能：取消线程(让线程终止)
            取消某个线程，可以终止某个线程的运行，但不是立即终止，而是当子线程执行到一个取消点
            线程才会终止，取消点：系统规定好的一些系统调用，我们可以初略额理解为用户区到内核区的切换，可以称为取消点
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
    pthread_t pid;
    int ret=pthread_create(&pid,NULL,callback,NULL);
#ifdef DEBUG
    if(ret!=0){
        printf("pthread_create:%s\n",strerror(ret));
        exit(0);
    }
#endif
    printf("tid: %ld ,main thread:%ld\n",pid,pthread_self());
   
   for(int i=0;i<5;i++){
    printf("%d\n",i);
   }
   //取消线程
   ret=pthread_cancel(pid);
#ifdef DEBUG
    if(ret!=0){
        printf("pthread_cancel:%s\n",strerror(ret));
        exit(0);
    }
#endif
    //让主线程退出，当主线程退出时，不会影响其他正常运行的线程
    pthread_exit(NULL);
    return 0;
}