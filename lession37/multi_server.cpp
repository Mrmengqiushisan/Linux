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
    //设置广播属性
    struct in_addr imr_multiaddr;
    inet_pton(AF_INET,"239.0.0.10",&imr_multiaddr.s_addr);
    setsockopt(sock_server,IPPROTO_IP,IP_MULTICAST_IF,&imr_multiaddr,sizeof(imr_multiaddr));
    //初始化地址信息
    sockaddr_in cliaddr;
    cliaddr.sin_family=AF_INET;
    cliaddr.sin_port=htons(9527);
    inet_pton(AF_INET,"239.0.0.10",&cliaddr.sin_addr.s_addr);
    //通信
    char buf[BUFFER_SZIE]={0};
    int num=0;
    while(1){
        sprintf(buf,"HELLO CLIENT %d\n",num++);
        //发送数据
        sendto(sock_server,buf,strlen(buf)+1,0,(sockaddr*)&cliaddr,sizeof(cliaddr));
        cout<<"组播的数据："<<buf<<endl;
        sleep(1);
    }
    close(sock_server);
    return 0;
}