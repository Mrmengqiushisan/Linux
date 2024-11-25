#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#define BUFFER_SZIE 1024
using namespace std;

int main(){
    //创建一个通信的socket
    int sock_server=socket(PF_INET,SOCK_DGRAM,0);
    if(sock_server==-1){
        perror("socket");
        exit(-1);
    }
    //绑定
    sockaddr_in seradr;
    seradr.sin_family=AF_INET;
    seradr.sin_port=htons(9527);
    seradr.sin_addr.s_addr=INADDR_ANY;
    if(bind(sock_server,(sockaddr*)&seradr,sizeof(seradr))==-1){
        perror("bind");
        exit(-1);
    }
    //通信
    char buf[BUFFER_SZIE]={0};
    while(1){
        //接受数据
        char ipbuf[16]={0};
        sockaddr_in cliadr;
        socklen_t len=sizeof(cliadr);
        int ret=recvfrom(sock_server,buf,sizeof(buf),0,(sockaddr*)&cliadr,&len);
        if(ret==-1){
            perror("recvfrom");
            break;
        }
        cout<<"client IP: "<<inet_ntop(AF_INET,&cliadr.sin_addr.s_addr,ipbuf,sizeof(ipbuf));
        cout<<"Port: "<<ntohs(cliadr.sin_port)<<endl;
        cout<<"client say: "<<buf<<endl;
        //发送数据
        sendto(sock_server,buf,strlen(buf)+1,0,(sockaddr*)&cliadr,sizeof(cliadr));
    }
    close(sock_server);
    return 0;
}