//实现客户端

#include<iostream>
#include<arpa/inet.h>
#include<string>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
using namespace std;


int main(){
    //创建套接字 
    int sock_server=socket(AF_INET,SOCK_STREAM,0);
#ifdef  DEBUG
    if(sock_server==-1){
        perror("socket");
    }
    else{
        cout<<"socket success:"<<sock_server<<endl;
    }
#endif
    //连接
    sockaddr_in seraddr;
    seraddr.sin_family=PF_INET;
    seraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    seraddr.sin_port=htons(9527);
    int ret=connect(sock_server,(sockaddr*)&seraddr,sizeof(seraddr));
#ifdef  DEBUG
    if(ret==-1){
        perror("connect");
    }
    else{
        cout<<"connect success"<<endl;
    }
#endif
    string buf;
    buf.resize(1024);
    char* buffer=(char*)buf.c_str();
    while(1){
        string buf="Please input msg\n";
        write(STDOUT_FILENO,buf.c_str(),buf.size());
        cin>>buffer;
        if(strcmp(buffer,"q")==0||strcmp(buffer,"Q")==0)break;
        int ret=send(sock_server,buffer,buf.size(),0);
        if(ret==-1){
            perror("send");
        }else if(ret==0){
            cout<<"sock has close"<<endl;
        }
        memset(buffer,0,sizeof(buffer));
        ret=recv(sock_server,buffer,buf.size(),0);
        cout<<buffer<<endl;
    }
    close(sock_server);
    return 0;
}