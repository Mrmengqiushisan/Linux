#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#define MAX_NUM 10
int GetCpuNum(){
    return 1;
}
int DoSomething(){
    return 1;
}
bool TestFunction(){
    int nArray[MAX_NUM];
    memset(&nArray,0,sizeof(nArray));
    int nValidNum=GetCpuNum();
    for(int i=0;i<nValidNum;i++){
        nArray[i]=DoSomething();
    }
    return true;
}
int main(void){
    TestFunction();
    return 0;
}