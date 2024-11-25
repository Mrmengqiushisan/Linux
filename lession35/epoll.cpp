

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
/*
    创建一个新的epoll实例;在内核中创建了一个数据，这个数据中有两个比较重要的数据;
        一个是需要检测的文件描述符的信息（红黑树）;
        还有一个是就绪列表，存放检测到数据发送改变的文件描述符信息(双向链表).
    int epoll_create(int size);
        - 参数：
            size : 目前没有意义了。随便写一个数，必须大于0
        - 返回值：
            -1 : 失败
            > 0 : 文件描述符，操作epoll实例的
    typedef union epoll_data{
        void *ptr;
        int  fd;
        uint32_t u32;
        uint32_t u64;
    }epoll_data_t;
    struct epoll_event{
        uint32_t events;        //epoll event
        epoll_data_t data;      //user data variable
    }
    常见的epoll检测事件
        -EPOLLIN
        -EPOLLOUT
        -EPOLLERR
    对epoll实例进行管理：添加文件描述符信息，删除信息，修改信息
    int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
        - 参数：
            - epfd : epoll实例对应的文件描述符
            - op : 要进行什么操作
                EPOLL_CTL_ADD: 添加
                EPOLL_CTL_MOD: 修改
                EPOLL_CTL_DEL: 删除
            - fd : 要检测的文件描述符
            - event : 检测文件描述符什么事情
    // 检测函数
    int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
        - 参数：
            - epfd : epoll实例对应的文件描述符
            - events : 传出参数，保存了发送了变化的文件描述符的信息
            - maxevents : 第二个参数结构体数组的大小
            - timeout : 阻塞时间
                - 0 : 不阻塞
                - -1 : 阻塞，直到检测到fd数据发生变化，解除阻塞
                - > 0 : 阻塞的时长（毫秒）
        - 返回值：
            - 成功，返回发送变化的文件描述符的个数 > 0
            - 失败 -1
    
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
                char buf[BUFFER_SIZE]={0};
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