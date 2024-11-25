#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#define BUFFER_SZIE 1024
using namespace std;

int main(){
    //创建一个通信的socket
    int sock_client=socket(PF_INET,SOCK_DGRAM,0);
    if(sock_client==-1){
        perror("socket");
        exit(-1);
    }
    //客户端绑定本地的IP和端口
    sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(9527);
    addr.sin_addr.s_addr=INADDR_ANY;
    if(bind(sock_client,(sockaddr*)&addr,sizeof(addr))==-1){
        perror("bind");
        exit(-1);
    }
    //加入到多播组
    struct ip_mreq op;
    inet_pton(AF_INET,"239.0.0.10",&op.imr_multiaddr.s_addr);
    op.imr_interface.s_addr=INADDR_ANY;
    setsockopt(sock_client,IPPROTO_IP,IP_ADD_MEMBERSHIP,&op,sizeof(op));
    //通信
    char buf[BUFFER_SZIE]={0};
    while(1){
        //接受数据
        int ret=recvfrom(sock_client,buf,sizeof(buf),0,NULL,NULL);
        if(ret==-1){
            perror("recvfrom");
            break;
        }
        cout<<"server say: "<<buf<<endl;
    }   
    close(sock_client);
    return 0;
}