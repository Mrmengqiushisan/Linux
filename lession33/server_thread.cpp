//共享虚拟地址空间：堆共享 栈不共享
#include<iostream>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/socket.h>
#define BUFFER_SIZE 1024

using namespace std;

typedef struct sockInfo{
    int m_fd;//通信的文件描述符
    pthread_t m_tid;//线程号
    struct sockaddr_in m_addr;
    sockInfo(){}
    sockInfo(const int& fd,const pthread_t& pd,const sockaddr_in& addr){
        m_fd=fd;
        m_tid=pd;
        memcpy(&m_addr,&addr,sizeof(addr));
    }
}SOCKINFO,*PSOCKINFO;

SOCKINFO sockinfos[BUFFER_SIZE];

void* working(void* arg){
    //子线程和客户端通信 客户端的信息  线程号
    //子进程
    //获取客户端的信息
    PSOCKINFO pinfo=(PSOCKINFO)arg;
    char cliIp[16];
    inet_ntop(AF_INET,&pinfo->m_addr.sin_addr.s_addr,cliIp,sizeof(cliIp));
    unsigned short cliPort=ntohs(pinfo->m_addr.sin_port);
    printf("client ip is :%s,port is:%d\n",cliIp,cliPort);
    //接受客户端发来的数据
    char recvBuf[BUFFER_SIZE]={0};
    while (1){
        int len=read(pinfo->m_fd,recvBuf,BUFFER_SIZE);
        if(len==-1){
            perror("read");
            pinfo->m_fd=-1;pinfo->m_tid=-1;
            exit(-1);
        }else if(len>0){
            printf("recv client data : %s\n",recvBuf);
        }else {
            cout<<"client closed..."<<endl;
            pinfo->m_fd=-1;pinfo->m_tid=-1;
            break;
        }
        len=write(pinfo->m_fd,recvBuf,strlen(recvBuf));
    }
    close(pinfo->m_fd);
    pthread_exit(nullptr);
}

int main(){
    //创建socket
    int sock_server=socket(PF_INET,SOCK_STREAM,0);
    if(sock_server==-1){
        perror("socket");
        exit(-1);
    }
    //端口复用
    int optval=1;
    setsockopt(sock_server,SOL_SOCKET,SO_REUSEPORT,&optval,sizeof(optval));
    //绑定
    sockaddr_in seraddr;
    seraddr.sin_family=AF_INET;
    inet_pton(AF_INET,"0.0.0.0",&seraddr.sin_addr.s_addr);
    seraddr.sin_port=htons(9527);
    int ret= bind(sock_server,(sockaddr*)&seraddr,sizeof(seraddr));
    if(ret==-1){
        perror("bind");
        exit(-1);
    }
    //监听
    ret=listen(sock_server,5);
    if(ret==-1){
        perror("listen");
        exit(-1);
    }
    //初始化数据
    for(int i=0;i<BUFFER_SIZE;i++){
        bzero(&sockinfos[i],sizeof(sockinfos[i]));
        sockinfos[i].m_fd=-1;
        sockinfos[i].m_tid=-1;
    }
    //不断循环等待客户端连接   
    while(1){
        sockaddr_in cliaddr;
        memset(&cliaddr,0,sizeof(cliaddr));
        socklen_t cliaddrLen=sizeof(cliaddr);
        int sock_client=accept(sock_server,(sockaddr*)&cliaddr,&cliaddrLen);
        //创建子线程
        PSOCKINFO pinfo;
        for(int i=0;i<BUFFER_SIZE;i++){
            if(sockinfos[i].m_fd==-1){
                pinfo=&sockinfos[i];
                break;
            }
            if(i==BUFFER_SIZE-1){
                sleep(1);
                i--;
            }
        }
        pinfo->m_fd=sock_client;
        memcpy(&pinfo->m_addr,&cliaddr,sizeof(cliaddr));
        pthread_create(&pinfo->m_tid,nullptr,working,pinfo);
        pthread_detach(pinfo->m_tid);
    }
    close(sock_server);
    pthread_exit(0);
    return 0;
}
