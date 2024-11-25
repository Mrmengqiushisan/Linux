#include<iostream>
#include"Mytool.h"
int main(){
   // int a=10;
#ifdef DEBUG
    std::cout<<"我是一个程序员，我不会爬树"<<std::endl;
#endif
    for(int i=0;i<3;i++){
        std::cout<<"hell gcc"<<std::endl;
    }
    MyTool tool;
    tool.GetAGE();
    tool.GetSex();
    return  0;
}
