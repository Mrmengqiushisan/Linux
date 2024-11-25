/*
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
int msgget(key_t key, int msgflg);
    -功能:获得一个与key值有关的消息队列，给队列中的内存不仅与该key标识之前的数据相关，同样也可以创建新的集合
    -参数:
        -key:用于标识唯一的消息对立额，不同的消息队列可以使用不同的KEY来区分，可以使用ftok()函数来生成唯一的key
        -msgflg:
            IPC_CREAT
            IPC_EXCL
    -返回值；
        成功：返回该消息队列的唯一标识符
        失败：返回-1
int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
    -功能：用于在消息队列中发送消息
    -参数：
        - msqid:消息队列的标识符
        - msgp:这是一个指向消息缓冲区的指针，其中包含要发送的消息。
        struct msgbuf {
               long mtype;      
               char mtext[1];    
        };
        - msgsz:这是消息的大小，以字节为单位
        - msgflg:这是一组标志，用于指定消息发送的行为。常见的标志包括
            IPC_NOWAIT：如果消息队列已满，不要阻塞，立即返回错误。
            0：阻塞模式，如果消息队列已满，等待直到有足够的空间发送消息。
    -返回值：
        成功：0
        失败：-1
ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp,int msgflg);
    -功能：用于消息队列数据的接受
    -参数：
        -msqid：消息队列的标识符
        -msgp：这是一个指向消息缓冲区的指针，用于存储接收到的消息内容
        -msgsz:缓冲区的大小
        -msgtype:消息类型
            >0:函数将尝试接收与给定类型匹配的第一条消息
            =0:函数将接收队列中的第一条消息
            <0:函数将尝试接收小于或等于 msgtyp 绝对值的最小消息类型
        -msgflg:
            IPC_NOWAIT：如果消息队列为空，不要阻塞，立即返回错误。
            MSG_EXCEPT：接收队列中不与 msgtyp 匹配的第一条消息。
    -返回值:
        成功：返回实际接收到的字节数
        失败：-1
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
struct msgbuf{
    long mtype;
    char mtext[100];
};

int main(){
    //创建一个消息队列的标识符
    key_t key_tok=ftok("/home/wangtao/Linux/lession27",'A');
    //创建一个消息队列
    int msg=msgget(key_tok,IPC_CREAT|0664);
#ifdef DEBUG
    if(msg==-1){
        perror("msgget");
        exit(0);
    }else{
        printf("msgget success\n");
    }
#endif
    //新建进程
    pid_t pid=fork();
    if(pid>0){
        //父进程   
        struct msgbuf message;
        message.mtype=1;
        strcpy(message.mtext,"hello message queue");
        int ret=msgsnd(msg,&message,sizeof(message),0);
#ifdef DEBUG
        if(ret==-1){
            perror("msgsnd:");
            exit(0);
        }else{
            printf("msgsnd success\n");
        }
#endif
    }else if(pid==0){
        //子进程
        struct msgbuf buf;
        ssize_t ret=msgrcv(msg,&buf,sizeof(buf),1,MSG_NOERROR|0);
#ifdef DEBUG
        if(ret==-1){
            perror("msgrecv");
            exit(0);
        }else{
            printf("msgrcv success\n");
        }
#endif
        printf("message: %s,message size:%d\n",buf.mtext,(int)ret);
    }
    return 0;
}