/*
    信号量的类型 sem_t
    int sem_init(sem_t *sem, int pshared, unsigned int value);
        - 功能 初始化一个信号量
        - 参数：
            - sem；信号量变量的地址
            - pshared：用于区别这个信号量用于线程还是进程之间
            - value：信号量中的值
    int sem_destroy(sem_t *sem);
        - 释放资源
    int sem_wait(sem_t *sem);
        - 对信号量的值减一 如果这个值为0 阻塞
    int sem_trywait(sem_t *sem);
    int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
    int sem_post(sem_t *sem);
        - 解锁一个信号量 调用一次 对信号量的值加一
    int sem_getvalue(sem_t *sem, int *sval)

    sem_t psem;
    sem_t csem;
    init(psem,0,8);
    init(csem,0,0);
    producer(){
        sem_wait(&psem);

        sem_post(&csem);
    }

    customer(){
        sem_wait(&csem);

        sem_post(&psem);
    }
*/

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<semaphore.h>
struct Node{
    int num;
    struct Node* next;
};

struct Node* head=NULL;
pthread_mutex_t mutex;
//创建两个信号量
sem_t psem;
sem_t csem;
void* producer(void* arg){
    //不断的创建新的结点
    while(1){
        sem_wait(&psem);
        pthread_mutex_lock(&mutex);
        struct Node* newNode=(struct Node*)malloc(sizeof(struct Node));
        newNode->next=head;
        head=newNode;
        newNode->num=rand()%1000;
        printf("add node,num:%d,tid:%ld\n",newNode->num,pthread_self());
        //只要生产了一个 就通知消费者消费
        pthread_mutex_unlock(&mutex);
        sem_post(&csem);
        usleep(100);
    }    
    return NULL;
}

void* customer(void* arg){
    while(1){
        sem_wait(&csem);
        pthread_mutex_lock(&mutex);
        struct Node* tmp=head;
        head=head->next;
        printf("delete node num:%d,tid:%ld\n",tmp->num,pthread_self());
        free(tmp);
        tmp=NULL;
        pthread_mutex_unlock(&mutex);
        sem_post(&psem);
        usleep(100);
    }
    return NULL;
}
int main(){
    pthread_mutex_init(&mutex,NULL);
    sem_init(&psem,0,8);
    sem_init(&csem,0,0);
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
    sem_destroy(&psem);
    sem_destroy(&csem);
    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);
    return 0;
}