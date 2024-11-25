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
    //绑定
    sockaddr_in seradr;
    seradr.sin_family=AF_INET;
    seradr.sin_port=htons(9527);
    inet_pton(AF_INET,"127.0.0.1",&seradr.sin_addr.s_addr);
    //通信
    char buf[BUFFER_SZIE]={0};
    int num=0;
    while(1){
         //发送数据
        sprintf(buf,"hello,i am client %d\n",num++);
        sendto(sock_client,buf,strlen(buf)+1,0,(sockaddr*)&seradr,sizeof(seradr));
        //接受数据
        char ipbuf[16]={0};
        sockaddr_in cliadr;
        socklen_t len=sizeof(cliadr);
        int ret=recvfrom(sock_client,buf,sizeof(buf),0,NULL,NULL);
        if(ret==-1){
            perror("recvfrom");
            break;
        }
        cout<<"server say: "<<buf<<endl;
        sleep(1);
    }   
    close(sock_client);
    return 0;
}