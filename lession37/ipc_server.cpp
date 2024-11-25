#include<iostream>
#include<unistd.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/un.h>
using namespace std;

int main(){
    unlink("server.sock");
    //创建监听的套接字
    int sock_server=socket(PF_LOCAL,SOCK_STREAM,0);
    if(sock_server==-1){
        perror("socket");
        exit(-1);
    }
    //绑定本地套接字文件
    sockaddr_un addr;
    addr.sun_family=AF_LOCAL;
    strcpy(addr.sun_path,"server.sock");
    if(bind(sock_server,(sockaddr*)&addr,sizeof(addr))==-1){
        perror("bind");
        exit(-1);
    }
    //监听
    if(listen(sock_server,100)==-1){
        perror("listen");
        exit(-1);
    }
    //等待客户端连接
    sockaddr_un cliaddr;
    socklen_t len=sizeof(cliaddr);
    int sock_client=accept(sock_server,(sockaddr*)&cliaddr,&len);
    if(sock_client==-1){
        perror("accept");
        exit(-1);
    }
    cout<<"client socket filename :"<<cliaddr.sun_path<<endl;
    //通信
    while(1){
        char buf[128]={0};
        int len=recv(sock_client,buf,sizeof(buf),0);
        if(len==-1){
            perror("recv");
            exit(-1);
        }else if(len==0){
            cout<<"client closed"<<endl;
            break;
        }else{
            cout<<"client say: "<<buf<<endl;
            send(sock_client,buf,len,0);
        }
    }
    close(sock_client);
    close(sock_server);
    return 0;
}