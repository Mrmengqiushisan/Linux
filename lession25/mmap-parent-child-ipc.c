/*
#include <sys/mman.h>
void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
    - 功能：将一个文件或者设备的数据映射到内存中
    - 参数：
        - void*addr:NULL 有内核决定
        - length: 要映射的数据的长度 这个值不能为零 建议使用文件的长度
            获取文件的长度：stat lseek
        - prot: 对申请的内存映射区的操作权限
            PROT_EXEC  可执行权限
            PROT_READ  读权限
            PROT_WRITE 写权限
            PROT_NONE  没有权限
            要操作映射区映射内存 必须要有读取权限
            PROT_READ PROT_READ|PROT_WRITE(一般都选择这个)
        - flags:
            - MAP_SHARED:映射区的数据会自动和磁盘文件进行同步必须要设置这个选项
            - MAP_PRIVATE:不同步 内存映射去的数据改变了，对原来的文件不会修改 会重新创建一个新的文件
        - fd: 我们需要映射的文件描述符号 通过open得到
            - 注意：文件的大小不能为零 open指定的权限不能和port参数有冲突
                PORT:PORT_READ open:只读/读写 
                PORT:PORT_READ|PORT_WRITE open:读写
        - offset: 偏移量 一般不用 必须指定4k的整数倍
    - 返回创建的内存的首地址
      失败返回 MAP_FAILED
int munmap(void *addr, size_t length);
    - 功能：释放内存映射
    - 参数：
        - addr: 要释放的内存的首地址
        - length: 要释放的内存的大小，要和mmap函数中的length参数的值一样
*/
/*
    使用内存映射实现进程间通信：
    1有关系的进程(父子进程)
        - 还没有子进程的时候创建一个内存映射区 
        - 有了内存映射区后创建子进程
        - 父子进程共享创建的内存映射区
    2没有关系的进程间同行
        - 准备一个大小不是零的磁盘文件
        - 进程1 通过磁盘文件创建内存映射区
            - 得到一个操作这块内存的指针
        - 进程2 通过磁盘文件创建内存映射区
            - 得到一个操作这块内存的指针
        - 使用内存映射区通信
    注意：内存映射区是非阻塞的
*/
#include<sys/mman.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<wait.h>
int main(){
    //打开一个文件
    int fd=open("./test.txt",O_RDWR);
    int size=lseek(fd,0,SEEK_END);
    //创建内存映射区
    void* ptr=mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(ptr==MAP_FAILED){
        perror("mmap");
        exit(0);
    }
    //创建子进程
    pid_t pid= fork();
    if(pid>0){
        wait(NULL);
        char buf[64]={0};
        strcpy(buf,(char*)ptr);
        printf("read data:%s\n",buf);
    }else if(pid==0){
        strcpy((char*)ptr,"nihao father!!!");
    }
    //关闭内存映射区
    munmap(ptr,size);
    return 0;
}