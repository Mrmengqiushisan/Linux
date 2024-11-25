

#include<iostream>
#include<arpa/inet.h>
#include<sys/select.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/poll.h>
#define BUFFER_SIZE 1024

/*
struct pollfd{
    int fd;             //委托内核检测的文件描述符
    short events;       //委托内核检测文件描述符的什么事件
    short revents;      //文件描述符实际发生的事件
}
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
    - 参数:
        -fds: 是一个struct pollfd 结构体数组 这是一个需要检测的文件描述符的集合
        -nfds: 这个是一个参数数组中最后一个有效元素的下标+1
        -timeout: 阻塞时长
            - 0: 不阻塞
            - -1: 阻塞，当检测到需要检测的文件描述符有变化，接触阻塞
            - >0: 阻塞的时长
    - 返回值:
        -1: 失败
        >0: 成功，n表示检测到集合中有n个文件描述符有变化
*/
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
    struct pollfd fds[BUFFER_SIZE];
    for(int i=0;i<BUFFER_SIZE;i++){
        fds[i].fd=-1;
        fds[i].events=POLLIN;
    }
    fds[0].fd=sock_server;
    int nfds{0};
    while(1){
        //调用poll系统函数 让内核检测那些文件描述符有数据 为什么第二项是+1，因为当前只输入了一个
        int ret=poll(fds,nfds+1,-1);
        if(ret==-1){
            perror("select");
            exit(-1);
        }else if(ret==0){
            continue;
        }else if(ret>0){
            //说明检测到了有文件描述符数据发生了改变
            if(fds[0].revents&POLLIN){
                //表示有新的客户端连接进来
                sockaddr_in cliadr;
                socklen_t cliadrLen=sizeof(cliadr);
                int sock_client=accept(sock_server,(sockaddr*)&cliadr,&cliadrLen);
                char buf[BUFFER_SIZE]={0};
                inet_ntop(AF_INET,&cliadr.sin_addr.s_addr,buf,sizeof(buf));
                unsigned short port=ntohs(cliadr.sin_port);
                printf("client IP: %s,Port: %d\n",buf,port);
                //将新的文件描述符加入到集合中
                for(int i=1;i<BUFFER_SIZE;i++){
                    if(fds[i].fd==-1){
                        fds[i].fd=sock_client;
                        fds[i].events=POLLIN;
                        break;
                    }
                }
                nfds=nfds>sock_client?nfds:sock_client;
            }
            for(int i=1;i<=nfds;i++){
                if(fds[i].revents&POLLIN){
                    //说明对应的文件描述符发来了数据
                    char buf[BUFFER_SIZE]={0};
                    int len=read(fds[i].fd,buf,sizeof(buf));
                    if(len==-1){
                        perror("read");
                        exit(-1);
                    }else if(len==0){
                        printf("client closed...\n");
                        close(fds[i].fd);
                        fds[i].fd=-1;
                    }else{
                        cout<<"Read buf :"<<buf<<endl;
                        write(fds[i].fd,buf,strlen(buf)+1);
                    }
                }
            }
        }
    }
    close(sock_server);
    return 0;
}