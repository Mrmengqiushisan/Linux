

#include<iostream>
#include<arpa/inet.h>
#include<sys/select.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/epoll.h>
#define BUFFER_SIZE 1024

using namespace std;

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
    //调用epoll_create()创建一个epoll示例
    int epfd=epoll_create(100);

    //将监听的文件描述符
    struct epoll_event epev;
    epev.events=EPOLLIN;
    epev.data.fd=sock_server;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sock_server,&epev);

    struct epoll_event epevs[BUFFER_SIZE];

    while(1){
        //调用poll系统函数 让内核检测那些文件描述符有数据
        int ret=epoll_wait(epfd,epevs,BUFFER_SIZE,-1);
        if(ret==-1){
            perror("epoll");
            exit(-1);
        }
        printf("ret=%d\n",ret);
        for(int i=0;i<ret;i++){
            int curfd=epevs[i].data.fd;
            //说明检测到了有文件描述符数据发生了改变
            if(curfd==sock_server){
                //表示有新的客户端连接进来
                sockaddr_in cliadr;
                socklen_t cliadrLen=sizeof(cliadr);
                int sock_client=accept(sock_server,(sockaddr*)&cliadr,&cliadrLen);
                char buf[BUFFER_SIZE]={0};
                inet_ntop(AF_INET,&cliadr.sin_addr.s_addr,buf,sizeof(buf));
                unsigned short port=ntohs(cliadr.sin_port);
                printf("client IP: %s,Port: %d\n",buf,port);
                //将新的文件描述符加入到集合中
                epev.events=EPOLLIN;
                epev.data.fd=sock_client;
                epoll_ctl(epfd,EPOLL_CTL_ADD,sock_client,&epev);
            }else{
                //说明对应的文件描述符发来了数据
                char buf[5]={0};
                int len=read(curfd,buf,sizeof(buf));
                if(len==-1){
                    perror("read");
                    exit(-1);
                }else if(len==0){
                    printf("client closed...\n");
                    epoll_ctl(epfd,EPOLL_CTL_DEL,curfd,nullptr);
                    close(curfd);
                }else{
                    cout<<"Read buf :"<<buf<<endl;
                    write(curfd,buf,strlen(buf)+1);
                }
            }
        }
    }
    close(sock_server);
    close(epfd);
    return 0;
}