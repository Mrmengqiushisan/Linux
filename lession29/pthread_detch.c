/*
int pthread_detach(pthread_t thread);
    - 功能：分离一个线程，被分离的线程在终止的时候，会自动释放资源返回给系统
        1 不能多次分离，会产生不可预料的行为
        2 不能去连接一个已经分离的线程，会报错
    - 参数：
        需要分离的线程的ID
    - 返回值：
        成功：返回0
        失败：返回错误标识符
*/

#include<pthread.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
void* callback(void* arg){
    printf("child thread:%ld\n",pthread_self());
    //sleep(3);
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
    for(int i=0;i<5;i++){
        printf("%d\n",i); 
    }
    printf("tid: %ld ,main thread:%ld\n",pid,pthread_self());
    //主线程调用pthread_detach进行子线程分离
    ret=pthread_detach(pid);
#ifdef DEBUG
    if(ret!=0){
        printf("pthread_join:%s\n",strerror(ret));
        exit(0);
    }
#endif
    //设置分离后 对分离的子线程进行一个连接
//     ret=pthread_join(pid,NULL);
// #ifdef DEBUG
//     if(ret!=0){
//         printf("pthread_join:%s\n",strerror(ret));
//         exit(0);
//     }
// #endif
    //让主线程退出，当主线程退出时，不会影响其他正常运行的线程
    pthread_exit(NULL);
    return 0;
}