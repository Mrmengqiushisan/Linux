/*
int pthread_join(pthread_t thread, void **retval);
    - 功能：和一个已经终止的线程进行连接
            回收子线程的资源 这个函数是阻塞函数,调用一次只能回收一个子线程
            一般在主线程中使用
    - 参数：
        - thread：需要回收的子线程的id
        - retval: 接受子线程退出时的返回值
    -返回值：
        - 成功：返回0
        - 失败：strerror(num);
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
    //主线程调用pthread_join回收子线程的资源
    int* thread_retval;
    ret=pthread_join(pid,(void**)&thread_retval);
#ifdef DEBUG
    if(ret!=0){
        printf("pthread_join:%s\n",strerror(ret));
        exit(0);
    }
#endif
    printf("exit data:%d\n",*thread_retval);
    printf("子线程回收资源成功\n");
    //让主线程退出，当主线程退出时，不会影响其他正常运行的线程
    pthread_exit(NULL);
    return 0;
}