#include<iostream>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/errno.h>
#define BUFFER_SIZE 1024

using namespace std;

void recyleChild(int arg){
    while(1){
        int ret=waitpid(-1,nullptr,WNOHANG);
        if(ret==-1){
            break;
        }else if(ret==0){
            //还有子进程活着
            break;
        }else if(ret>0){
            cout<<"子进程 "<<ret<<" 被回收"<<endl;
        }
    }
}

int main(){
    //注册信号捕捉
    struct sigaction act;
    act.sa_flags=0;
    sigemptyset(&act.sa_mask);
    act.sa_handler=recyleChild;
    sigaction(SIGCHLD,&act,NULL);
    //创建socket
    int sock_server=socket(PF_INET,SOCK_STREAM,0);
    if(sock_server==-1){
        perror("socket");
        exit(-1);
    }
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
    //不断循环等待客户端连接
    while(1){
        //接受连接
        sockaddr_in cliaddr;
        memset(&cliaddr,0,sizeof(cliaddr));
        socklen_t cliaddrLen=sizeof(cliaddr);
        int sock_client=accept(sock_server,(sockaddr*)&cliaddr,&cliaddrLen);
        if(sock_client==-1){
            if(errno==EINTR){
                continue;
            }
            perror("accept");
            exit(-1);
        }
        //每一个连接进来，创建一个子进程和客户端进行通信
        pid_t pid=fork();
        if(pid==0){
            //子进程
            //获取客户端的信息
            char cliIp[16];
            inet_ntop(AF_INET,&cliaddr.sin_addr.s_addr,cliIp,sizeof(cliIp));
            unsigned short cliPort=ntohs(cliaddr.sin_port);
            printf("client ip is :%s,port is:%d\n",cliIp,cliPort);
            //接受客户端发来的数据
            char recvBuf[BUFFER_SIZE]={0};
            while (1){
                int len=read(sock_client,recvBuf,BUFFER_SIZE);
                if(len==-1){
                    perror("read");
                    exit(-1);
                }else if(len>0){
                    printf("recv client data : %s\n",recvBuf);
                }else {
                    cout<<"client closed..."<<endl;
                    exit(0);
                }
                len=write(sock_client,recvBuf,strlen(recvBuf));
            }
            close(sock_client);
            exit(0);//退出当前进程
        }
    }
    close(sock_server);
    return 0;
}