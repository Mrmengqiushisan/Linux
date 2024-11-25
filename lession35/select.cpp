#include<iostream>
#include<arpa/inet.h>
#include<sys/select.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#define BUFFER_SIZE 1024

using namespace std;

/*
int select(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, struct timeval *timeout);
    - 参数：
        - nfds: 委托内核检测的最大文件描述符的值+1
        - readfds: 要检测的文件描述符的读的集合，委托内核检测那些文件描述符的读的属性
            - 一般检测读操作
            - 对应的是对方发送过来的数据,因为读是被动的接收数据，检测的就是读缓冲
            - 是一个传入传出参数 传入代表需要检测的文件描述符  传出种会标记那些发生了读的文件描述符
        - writefds: 要检测的文件描述符的写的集合，委托内核检测那些文件描述符的写的属性
            - 委托内核检测写缓冲区是不是可以写数据
        - exceptfds: 检测发生异常的文件描述符的集合
        - timeout: 设置的超时时间
            struct timeval{
                long tv_sec;
                long tv_usec;
            }
            - NULL: 永久阻塞，直到检测到了文件描述符有变化
            - tv_sec=0,tv_usec=0 不阻塞
            - tv_sec=>0,tv_usec>0 阻塞对应的时间
    - 返回值
        - -1：失败
        - >0: 检测的集合中有n个文件描述符发生了变化
void FD_CLR(int fd,fd_set* set);
    - 将参数文件描述符fd对应的标志位设置为0
int  FD_ISSET(int fd,fd_set* set);
    - 判断fd对应的标志位是0还是1 
void FD_SET(int fd,fd_set* set);
    - 将参数文件描述符fd 对应的标志位 设置为1
*/


int main(){

    int sock_server=socket(PF_INET,SOCK_STREAM,0);
    sockaddr_in seradr;
    seradr.sin_family=AF_INET;
    seradr.sin_addr.s_addr=INADDR_ANY;
    seradr.sin_port=htons(9527);
    //设置端口复用
    int optval=1;
    setsockopt(sock_server,SOL_SOCKET,SO_REUSEPORT,&optval,sizeof(optval));
    if(bind(sock_server,(sockaddr*)&seradr,sizeof(seradr))==-1){
        perror("bind");
        exit(-1);
    }
    if(listen(sock_server,5)==-1){
        perror("listen");
        exit(-1);
    }
    //创建一个fd_set的集合
    fd_set rdset,tmp;
    FD_ZERO(&rdset);
    FD_SET(sock_server,&rdset);
    int maxfd=sock_server;
    while(1){
        tmp=rdset;
        //调用select系统函数 让内核检测那些文件描述符有数据
        int ret=select(maxfd+1,&tmp,nullptr,nullptr,nullptr);
        if(ret==-1){
            perror("select");
            exit(-1);
        }else if(ret==0){
            continue;
        }else if(ret>0){
            //说明检测到了有文件描述符数据发生了改变
            if(FD_ISSET(sock_server,&tmp)){
                //表示有新的客户端连接进来
                sockaddr_in cliadr;
                socklen_t cliadrLen=sizeof(cliadr);
                int sock_client=accept(sock_server,(sockaddr*)&cliadr,&cliadrLen);
                char buf[BUFFER_SIZE]={0};
                inet_ntop(AF_INET,&cliadr.sin_addr.s_addr,buf,sizeof(buf));
                unsigned short port=ntohs(cliadr.sin_port);
                printf("client IP: %s,Port: %d\n",buf,port);
                //将新的文件描述符加入到集合中
                FD_SET(sock_client,&rdset);
                maxfd=maxfd>sock_client?maxfd:sock_client;
            }
            for(int i=sock_server+1;i<=maxfd;i++){
                if(FD_ISSET(i,&tmp)){
                    //说明对应的文件描述符发来了数据
                    char buf[BUFFER_SIZE]={0};
                    int len=read(i,buf,sizeof(buf));
                    if(len==-1){
                        perror("read");
                        exit(-1);
                    }else if(len==0){
                        printf("client closed...\n");
                        close(i);
                        FD_CLR(i,&rdset);
                    }else{
                        cout<<"Read buf :"<<buf<<endl;
                        write(i,buf,strlen(buf)+1);
                    }
                }
            }
        }
    }
    close(sock_server);
    return 0;
}