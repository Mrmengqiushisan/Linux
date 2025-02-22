/*
条件变量的类型 pthread_cond_t
int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);
int pthread_cond_destroy(pthread_cond_t *cond);
int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
    - 阻塞函数 调用该函数线程会阻塞 等待
int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime);
    - 等待多长时间 调用了这个函数 线程会阻塞 直到直到的时间结束
int pthread_cond_signal(pthread_cond_t *cond);
    - 唤醒一个或者多个等地的线程
int pthread_cond_broadcast(pthread_cond_t *cond);
    - 唤醒所有的等待的线程
*/

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

struct Node{
    int num;
    struct Node* next;
};

struct Node* head=NULL;
pthread_mutex_t mutex;
//创建条件变量
pthread_cond_t cond;
void* producer(void* arg){
    //不断的创建新的结点
    while(1){
        pthread_mutex_lock(&mutex);
        struct Node* newNode=(struct Node*)malloc(sizeof(struct Node));
        newNode->next=head;
        head=newNode;
        newNode->num=rand()%1000;
        printf("add node,num:%d,tid:%ld\n",newNode->num,pthread_self());
        //只要生产了一个 就通知消费者消费
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        usleep(100);
    }    
    return NULL;
}

void* customer(void* arg){
    while(1){
        pthread_mutex_lock(&mutex);
        if(head==NULL){//没有数据 需要等待
            //当这个函数调用阻塞的时候 会对互斥锁进行解锁 当不阻塞的时候 会重新加锁
            pthread_cond_wait(&cond,&mutex);
            pthread_mutex_unlock(&mutex);
            continue;
        } 
        struct Node* tmp=head;
        head=head->next;
        printf("delete node num:%d,tid:%ld\n",tmp->num,pthread_self());
        free(tmp);
        tmp=NULL;
        pthread_mutex_unlock(&mutex);
        usleep(100);
    }
    return NULL;
}
int main(){
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    //创建5个生产者线程,和5个消费者线程
    pthread_t ptids[5]={0};
    pthread_t ctids[5]={0};
    for(int i=0;i<5;i++){
        pthread_create(&ptids[i],NULL,producer,NULL);
        pthread_create(&ctids[i],NULL,customer,NULL);
    }
    for(int i=0;i<5;i++){
        pthread_join(ptids[i],NULL);
        pthread_join(ctids[i],NULL);
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_exit(NULL);
    return 0;
}