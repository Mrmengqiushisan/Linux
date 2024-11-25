// 发送端
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
 
 
using namespace std;
 
int main()
{
	setvbuf(stdout, NULL, _IONBF, 0); 
	fflush(stdout); 
 
	int sock = -1;
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
	{   
		cout<<"socket error"<<endl;	
		return false;
	}   
	
	// const int opt = 1;
	// //设置该套接字为广播类型，
	// int nb = 0;
	// nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
	// if(nb == -1)
	// {
	// 	cout<<"set socket error..."<<endl;
	// 	return false;
	// }
    struct sockaddr_in addrself;
    bzero(&addrself,sizeof(struct sockaddr_in));
    addrself.sin_family=AF_INET;
    addrself.sin_addr.s_addr=htonl(INADDR_ANY);
    addrself.sin_port=htons(6000);
    if(bind(sock,(sockaddr*)&addrself,sizeof(addrself))==-1){
        cout<<"bind error:"<<endl;
        return -1;
    }
	struct sockaddr_in addrto;
	bzero(&addrto, sizeof(struct sockaddr_in));
	addrto.sin_family=AF_INET;
	addrto.sin_addr.s_addr=htonl(INADDR_ANY);
	addrto.sin_port=htons(6000);
	int nlen=sizeof(addrto);
    char smsg[100]={};
	while(1)
	{
		sleep(1);
		//从广播地址发送消息
        int ret=recvfrom(sock,smsg,100,0,(sockaddr*)&addrto,(socklen_t*)&nlen);
		if(ret<0)
		{
			cout<<"send error...."<<ret<<endl;
		}
		else
		{		
			printf("%s\t",smsg);	
		}
	}
 
	return 0;
}
