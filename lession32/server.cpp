//实现socket服务端
//实现回射通话
#include<iostream>
#include<arpa/inet.h>
#include<string.h>
#include<fcntl.h>
using namespace std;

int main(){
    //创建套接字描述符
    int sock_server=socket(AF_INET,SOCK_STREAM,0);
#ifdef  DEBUG
    if(sock_server==-1){
        perror("socket");
    }
    else{
        cout<<"socket success:"<<sock_server<<endl;
    }
#endif
    //建立连接
    sockaddr_in seraddr;
    seraddr.sin_family=PF_INET;
    seraddr.sin_addr.s_addr=inet_addr("0.0.0.0");
    seraddr.sin_port=htons(9527);
    int ret=bind(sock_server,(sockaddr*)&seraddr,sizeof(seraddr));
#ifdef  DEBUG
    if(ret==-1){
        perror("bind");
    }
    else{
        cout<<"bind success"<<endl;
    }
#endif
    //监听
    ret=listen(sock_server,5);
#ifdef  DEBUG
    if(ret==-1){
        perror("listen");
    }
    else{
        cout<<"listen success"<<endl;
    }
#endif
    //接受阻塞
    sockaddr_in cliaddr;
    memset(&cliaddr,0,sizeof(cliaddr));
    socklen_t cliaddrlen=sizeof(cliaddr);
    int sock_client=accept(sock_server,(sockaddr*)&cliaddr,&cliaddrlen);
    //业务逻辑
#ifdef  DEBUG
    if(sock_client==-1){
        perror("accept");
    }
    else{
        cout<<"accept success:"<<sock_client<<endl;
    }
#endif
    char buf[1024]={0};
    while(1){
        int ret=recv(sock_client,buf,sizeof(buf),0);
        if(ret==-1){
            perror("recv");
        }
        else if(ret==0){
            cout<<"client close"<<endl;
            break;
        }
        cout<<"come from client:"<<buf<<" size:"<<ret<<endl;
        ret=send(sock_client,buf,strlen(buf),0);
        if(ret==-1){
            perror("send:");
        }
    }
    
    return 0;
}