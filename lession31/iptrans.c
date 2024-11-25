/*
#include <arpa/inet.h>
// p:点分十进制的IP字符串，n:表示network，网络字节序的整数
int inet_pton(int af, const char *src, void *dst);
af:地址族： AF_INET AF_INET6
src:需要转换的点分十进制的IP字符串
dst:转换后的结果保存在这个里面
// 将网络字节序的整数，转换成点分十进制的IP地址字符串
const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
af:地址族： AF_INET AF_INET6
src: 要转换的ip的整数的地址
dst: 转换成IP地址字符串保存的地方
size：第三个参数的大小（数组的大小）
返回值：返回转换后的数据的地址（字符串），和 dst 是一样的
*/

#include<stdio.h>
#include<arpa/inet.h>

int main(){
    //创建一个IP字符串
    char buf[]="192.168.23.21";
    unsigned int num=0;
    inet_pton(AF_INET,buf,&num);
    unsigned char* p=(unsigned char*)&num;
    for(int i=0;i<sizeof(num);i++){
        printf("%d  ",p[i]);
    }
    printf("\n");

    char buffer[16];
    const char* str= inet_ntop(AF_INET,&num,buffer,sizeof(buffer));
    printf("str: %s\n",str);
    printf("buf: %s\n",buffer);
    return 0;
}