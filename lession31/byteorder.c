/*
    字节序：字节在内存中存储的顺序
    小端字节序：数据的高位内存存储在内存的高位地址，低位地址存储的内存的低位地址
    大端字节序：数据的低位内存存储在内存的高位地址，高位地址存储的内存的低位地址
*/


#include<stdio.h>
#include<stdio.h>

void Dump(int value){
    char* by=(char*)&value;
    for(int i=0;i<sizeof(value);i++){
        printf("%02X ",by[i]);
    }
}


int main(){
    union{
        short value;
        char  bytes[sizeof(short)];
    }test;
    test.value=0x0102;
    if((test.bytes[0]==1)&&(test.bytes[1]==2)){
        printf("大端字节序\b");
    }else if((test.bytes[0]==2)&&(test.bytes[1]==1)){
        printf("小端字节序\n");
    }
    int num=0x01020304;
    Dump(num);
    return 0;
}