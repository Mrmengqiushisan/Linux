#include<iostream>
#include<unistd.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/un.h>
using namespace std;

int main(){
    unlink("client.sock");
    //创建套接字
    int sock_client=socket(PF_LOCAL,SOCK_STREAM,0);
    if(sock_client==-1){
        perror("socket");
        exit(-1);
    }
    //绑定本地套接字文件
    sockaddr_un addr;
    addr.sun_family=AF_LOCAL;
    strcpy(addr.sun_path,"client.sock");
    if(bind(sock_client,(sockaddr*)&addr,sizeof(addr))==-1){
        perror("bind");
        exit(-1);
    }
    //连接服务器
    sockaddr_un seraddr;
    seraddr.sun_family=AF_LOCAL;
    strcpy(seraddr.sun_path,"server.sock");
    if(connect(sock_client,(sockaddr*)&seraddr,sizeof(seraddr))==-1){
        perror("connect");
        exit(-1);
    }
    cout<<"server socket filename :"<<seraddr.sun_path<<endl;
    //通信
    int num{0};
    while(1){
        char buf[128]={0};
        sprintf(buf,"hello i am client : %d\n",num++);
        send(sock_client,buf,sizeof(buf),0);
        int len=recv(sock_client,buf,sizeof(buf),0);
        if(len==-1){
            perror("recv");
            exit(-1);
        }else if(len==0){
            cout<<"server closed"<<endl;
        }else{
            printf("server say:%s\n",buf);
        }
        sleep(1);
    }
    close(sock_client);
    return 0;
}