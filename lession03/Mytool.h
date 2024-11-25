#pragma once

class MyTool{
public:
    MyTool()=default;
    ~MyTool()=default;
    int GetAGE();
    int GetSex();
private:
    int age_;
    int sex_;    
};