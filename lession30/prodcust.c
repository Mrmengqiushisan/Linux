/*
    生产者消费者模型
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

void* producer(void* arg){
    //不断的创建新的结点
    while(1){
        pthread_mutex_lock(&mutex);
        struct Node* newNode=(struct Node*)malloc(sizeof(struct Node));
        newNode->next=head;
        head=newNode;
        newNode->num=rand()%1000;
        printf("add node,num:%d,tid:%ld\n",newNode->num,pthread_self());
        pthread_mutex_unlock(&mutex);
        usleep(100);
    }    
    return NULL;
}

void* customer(void* arg){
    while(1){
        pthread_mutex_lock(&mutex);
        if(head==NULL){
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
    pthread_exit(NULL);
    return 0;
}